#include <android/log.h>
#include <jni.h>
#include <cstdio>
#include "zygisk.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

#define TAG "MonetZygisk"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

class MonetModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        api_ = api;
        env_ = env;
        LOGI("onLoad: module loaded");
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        if (!env_ || !args || !args->nice_name) return;
        const char *pkg = env_->GetStringUTFChars(args->nice_name, nullptr);
        if (pkg) {
            LOGI("preAppSpecialize: %s", pkg);
            env_->ReleaseStringUTFChars(args->nice_name, pkg);
        }
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {}

    void preServerSpecialize(ServerSpecializeArgs *args) override {
        LOGI("preServerSpecialize: system_server");
    }

    void postServerSpecialize(const ServerSpecializeArgs *args) override {}

private:
    Api *api_ = nullptr;
    JNIEnv *env_ = nullptr;
};

REGISTER_ZYGISK_MODULE(MonetModule)
