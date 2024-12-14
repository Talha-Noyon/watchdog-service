#ifndef LINUX_API_H
#define LINUX_API_H

#include "watchdog/platform_api.h"

class LinuxAPI : public PlatformAPI {
    public:
        // Retrieves a list of running processes on the system.
        std::vector<ProcessInfo> getRunningProcesses() override;
        // Starts a process with the given command and arguments.
        bool startProcess(const std::string& command, const std::vector<std::string>& args) override;
    private:
        LinuxAPI() {} // Private constructor to prevent direct instantiation

        friend PlatformAPI& PlatformAPI::create(); // Allow `PlatformAPI` to create an instance

};


#endif // LINUX_API_H