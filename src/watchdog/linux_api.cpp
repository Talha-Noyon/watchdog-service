#ifdef __linux__
#include "watchdog/linux_api.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
namespace fs = std::filesystem;

std::vector<ProcessInfo> LinuxAPI::getRunningProcesses() {
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

bool LinuxAPI::startProcess(const std::string& command, const std::vector<std::string>& args) {
    std::string cmd = command;
    for (const auto& arg : args) {
        cmd += " " + arg;
    }
    return std::system(cmd.c_str()) == 0;
}

#endif