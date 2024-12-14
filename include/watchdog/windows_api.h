#ifndef WINDOWS_API_H
#define WINDOWS_API_H

#include "watchdog/platform_api.h"

class WindowsAPI : public PlatformAPI {
    public:
        // Retrieves a list of running processes on the system.
        std::vector<ProcessInfo> getRunningProcesses() override;
        // Starts a process with the given command and arguments.
        bool startProcess(const std::string& command, const std::vector<std::string>& args) override;
    private:
        WindowsAPI() {} // Private constructor to prevent direct instantiation

        friend PlatformAPI& PlatformAPI::create(); // Allow `PlatformAPI` to create an instance
};

#endif // WINDOWS_API_H
