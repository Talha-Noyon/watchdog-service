#ifndef LINUX_API_H
#define LINUX_API_H

#include "watchdog/platform_api.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <cstdlib>

namespace fs = std::filesystem;

class LinuxAPI : public PlatformAPI {
    public:
        std::vector<ProcessInfo> getRunningProcesses() override {
            std::vector<ProcessInfo> processes;

            for (const auto& entry : fs::directory_iterator("/proc")) {
                if (fs::is_directory(entry)) {
                    std::string pid = entry.path().filename().string();
                    if (std::all_of(pid.begin(), pid.end(), ::isdigit)) {
                        std::string cmd_path = entry.path().string() + "/exe";
                        try {
                            std::string resolved_path = fs::read_symlink(cmd_path).string();
                            processes.push_back({pid, resolved_path});
                        } catch (const std::exception&) {
                        }
                    }
                }
            }

            return processes;
        }

        bool startProcess(const std::string& command, const std::vector<std::string>& args) override {
            std::string cmd = command;
            for (const auto& arg : args) {
                cmd += " " + arg;
            }
            return std::system(cmd.c_str()) == 0;
        }

        bool stopProcess(const std::string& pid) override {
            std::string command = "kill " + pid;
            return std::system(command.c_str()) == 0;
        }

        bool isProcessRunning(const std::string& pid) override {
            std::string proc_path = "/proc/" + pid;
            return std::filesystem::exists(proc_path);
        }
    private:
        LinuxAPI() {} // Private constructor to prevent direct instantiation

        friend PlatformAPI& PlatformAPI::create(); // Allow `PlatformAPI` to create an instance

};


#endif // LINUX_API_H