#include "watchdog/watchdog.h"
#include "utils/helper.h"

#include <fstream>
#include <iostream>
#include <thread>
#include <memory>
#include <algorithm>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void Watchdog::loadConfiguration() {
    std::ifstream file(configFilePath);
    if (!file) {
        logEvent("Error: Unable to read configuration file: " + configFilePath);
        return;
    }

    try {
        json config;
        file >> config;
        file.close();

        std::unordered_map<std::string, std::string> newProcesses;
        for (const auto& process : config["processes"]) {
            newProcesses[process["name"]] = process["command"];
        }

        monitoredProcesses = std::move(newProcesses);
        logEvent("Configuration loaded successfully.");
    } catch (const std::exception& e) {
        logEvent("Error parsing configuration file: " + std::string(e.what()));
    }
}

void Watchdog::monitorProcesses() {
    for (const auto& [name, command] : monitoredProcesses) {
        if (!isProcessRunning(command)) {
            logEvent(templateLiteral("{} is not running. Restarting...", name));
            if (platformApi.startProcess(command, {})) {
                logEvent(templateLiteral("{} restarted successfully.", name));
            } else {
                logEvent(templateLiteral("Failed to restart {}.", name));
            }
        }
    }
}

bool Watchdog::isProcessRunning(const std::string& command) {
    auto processes = platformApi.getRunningProcesses();
    return std::any_of(processes.begin(), processes.end(), [&](const ProcessInfo& proc) {
        return proc.path == command;
    });
}

void Watchdog::updateConfig(const std::string& processName, const std::string& commandPath) {
    try {
        json config;
        std::ifstream configFile(configFilePath);

        if (configFile) {
            configFile >> config;
            configFile.close();
        }

        if (!config.contains("processes") || !config["processes"].is_array()) {
            config["processes"] = json::array();
        }

        config["processes"].push_back({{"name", processName}, {"command", commandPath}});
        std::ofstream updatedConfigFile(configFilePath);
        updatedConfigFile << config.dump(4);

        logEvent("Updated configuration with process: " + processName);
    } catch (const std::exception& e) {
        logEvent("Error updating configuration: " + std::string(e.what()));
    }
}

std::vector<int> Watchdog::selectProcess(const std::vector<ProcessInfo>& processes) {
    std::vector<int> selected;
    std::string input;

    std::cout << "Available processes:\n";
    for (size_t i = 0; i < processes.size(); ++i) {
        std::cout << "ID=" << i << ", Path=" << processes[i].path << "\n";
    }

    while (true) {
        std::cout << "Enter process ID (or type 'done' to finish): ";
        std::getline(std::cin, input);
        if (input == "done") break;

        try {
            int id = std::stoi(input);
            if (id >= 0 && id < static_cast<int>(processes.size())) {
                selected.push_back(id);
            } else {
                std::cout << "Invalid ID.\n";
            }
        } catch (...) {
            std::cout << "Invalid input.\n";
        }
    }
    return selected;
}

std::string Watchdog::getTimestamp() const {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return buf;
}

void Watchdog::logEvent(const std::string& message) const {
    std::ofstream logFile("log/watchdog.log", std::ios::app);
    logFile << getTimestamp() << " - " << message << "\n";
}

void Watchdog::initializeFromSelection() {
    auto processes = platformApi.getRunningProcesses();
    auto selected = selectProcess(processes);

    for (int id : selected) {
        auto& process = processes[id];
        updateConfig("selected_process_" + process.pid, process.path);
    }

    std::cout << "Processes added to configuration.\n";
}

void Watchdog::run() {
    while (true) {
        auto now = std::chrono::steady_clock::now();
        // Periodically reload configuration (e.g., every 10 seconds)
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastConfigCheck).count() >= 10) {
            loadConfiguration();
            lastConfigCheck = now;
        }
        monitorProcesses();
        // Sleep for a short duration before checking again
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}