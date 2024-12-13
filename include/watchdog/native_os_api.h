#include <string>
#include <vector>
#include <stdexcept>
#include <ctime>
#include "watchdog/process_info.h"
#include "watchdog/windows_api.h"
class NativeOSAPI
{
public:
    static NativeOSAPI &getOSAPI()
    {
// #ifdef __linux__
//         static LinuxOSApi linux_api;
//         return linux_api;
// #endif
#ifdef _WIN32
        static WindowsOsApi win_api;
        return win_api;
#else
        throw std::runtime_error("Unsupported OS");
#endif
    }
    std::string getTimestamp()
    {
        std::time_t now = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return buf;
    }
    virtual std::vector<ProcessInfo> getRunningProcesses() = 0;
    virtual bool isProcessRunning(const std::string process_name) = 0;
    virtual void startProcess(const std::string command) = 0;
    virtual bool stopProcess(const std::string process_name) = 0;
    virtual void logEvent(const std::string message) = 0;
    virtual ~NativeOSAPI() = default;
};
