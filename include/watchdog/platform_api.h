#ifndef PLATFORM_API_H
#define PLATFORM_API_H

#include <string>
#include <vector>

struct ProcessInfo {
    std::string pid;
    std::string path;
};

class PlatformAPI {
    public:
        virtual ~PlatformAPI() = default;

        // Abstract methods
        virtual std::vector<ProcessInfo> getRunningProcesses() = 0;
        virtual bool startProcess(const std::string& command, const std::vector<std::string>& args) = 0;

        // Factory method to create appropriate implementation
        static PlatformAPI& create();
};

#endif // PLATFORM_API_H
