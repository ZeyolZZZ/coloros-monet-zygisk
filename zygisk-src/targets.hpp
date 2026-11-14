// 由设备上 29 个 RRO overlay APK 的 manifest 实测得出（androguard 解析）
// 28 个不同的 targetPackage；SystemUI 有两个 overlay 但目标相同
#pragma once

static const char *kTargets[] = {
    "com.android.systemui",                 // SystemUI_single_monet, NotificationSidePadding
    "com.android.settings",                 // Setting
    "com.android.launcher",                 // Launcher
    "com.android.phone",
    "com.android.contacts",
    "com.coloros.alarmclock",
    "com.coloros.floatassistant",
    "com.coloros.oshare",
    "com.coloros.childrenspace",
    "com.coloros.digitalwellbeing",
    "com.coloros.accessibilityassistant",
    "com.coloros.phonemanager",
    "com.coloros.sharescreen",
    "com.oplus.camera",
    "com.oplus.battery",
    "com.oplus.ocar",
    "com.oplus.eyeprotect",
    "com.oplus.linker",
    "com.oplus.notificationmanager",
    "com.oplus.securitypermission",
    "com.oplus.pscanvas",
    "com.oplus.safecenter",
    "com.oplus.wirelesssettings",
    "com.heytap.speechassist",
    "com.heytap.browser",
    "com.microsoft.appmanager",
    "com.oneplus.oshare",
    "oplus",
};

static const int kTargetCount = (int)(sizeof(kTargets) / sizeof(kTargets[0]));
