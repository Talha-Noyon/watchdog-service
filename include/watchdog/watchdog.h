#ifndef WATCHDOG_API_H
#define WATCHDOG_API_H

#include "watchdog/platform_api.h"
#include <unordered_map>
#include <chrono>
class Watchdog {
    private:
        PlatformAPI& platformApi = PlatformAPI::create();
        std::string configFilePath;
        std::unordered_map<std::string, std::string> monitoredProcesses;
        std::chrono::time_point<std::chrono::steady_clock> lastConfigCheck;

        void loadConfiguration();
        void monitorProcesses();

        bool isProcessRunning(const std::string& command);

        void updateConfig(const std::string& processName, const std::string& commandPath);

        std::vector<int> selectProcess(const std::vector<ProcessInfo>& processes);

        std::string getTimestamp() const;

        void logEvent(const std::string& message) const;

    public:
        Watchdog(const std::string& configPath)
            : configFilePath(configPath), lastConfigCheck(std::chrono::steady_clock::now()) {
            loadConfiguration();
        }

        void initializeFromSelection();

        void run();
};

#endif // WATCHDOG_API_H