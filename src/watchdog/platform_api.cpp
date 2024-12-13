#include "watchdog/platform_api.h"
#ifdef _WIN32
#include "watchdog/windows_api.h"
#else
    #include "watchdog/linux_api.h"
#endif

PlatformAPI& PlatformAPI::create() {
#ifdef _WIN32
    static WindowsAPI win_api;
    return win_api;
#else
    static LinuxAPI linux_api;
    return linux_api;
#endif
}