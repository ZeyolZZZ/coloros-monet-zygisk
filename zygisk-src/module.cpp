#include <android/log.h>
#include <jni.h>
#include <string>
#include <unistd.h>
#include <cstdint>
#include "zygisk.hpp"
#include "targets.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

#define TAG "MonetZygisk"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// 请求 daemon 在【当前进程】挂载 overlay 的命令码
enum : uint32_t {
    CMD_MOUNT_OVERLAYS = 1,
};

// 判断包名是否为目标（支持 "pkg:process" 形式）
static bool isTarget(const std::string &name) {
    std::string pkg = name;
    auto colon = pkg.find(':');
    if (colon != std::string::npos) pkg.resize(colon);
    for (int i = 0; i < kTargetCount; i++) {
        if (pkg == kTargets[i]) return true;
    }
    return false;
}

class MonetModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        api_ = api;
        env_ = env;
        // 关键：钩子跑完后让 Zygisk 卸载本模块的 .so，
        // 避免它长期留在进程内存里被扫描成"匿名可执行映射"。
        api_->setOption(zygisk::Option::DLCLOSE_MODULE_LIBRARY);
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        if (!env_ || !args || !args->nice_name) return;
        const char *n = env_->GetStringUTFChars(args->nice_name, nullptr);
        if (!n) return;
        std::string name(n);
        env_->ReleaseStringUTFChars(args->nice_name, n);

        // system_server 由 preServerSpecialize 处理，这里跳过
        if (!isTarget(name)) return;

        LOGI("target hit: %s", name.c_str());

        // 拿 root companion 的 socket
        int fd = api_->connectCompanion();
        if (fd < 0) {
            LOGE("connectCompanion failed for %s", name.c_str());
            return;
        }
        // TODO(下一步): 发送 CMD_MOUNT_OVERLAYS + 模块目录 fd，由 daemon 完成挂载
        ::close(fd);
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {}
    void preServerSpecialize(ServerSpecializeArgs *args) override {}
    void postServerSpecialize(const ServerSpecializeArgs *args) override {}

private:
    Api *api_ = nullptr;
    JNIEnv *env_ = nullptr;
};

// companion 入口：daemon 侧会在这里被调用（下一步实现挂载逻辑）
static void companion_handler(int) {
    LOGI("companion_handler entered");
}

REGISTER_ZYGISK_MODULE(MonetModule)
REGISTER_ZYGISK_COMPANION(companion_handler)
