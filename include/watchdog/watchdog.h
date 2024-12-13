#include "watchdog/platform_api.h"
#include <unordered_map>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <iostream>
#include <bits/this_thread_sleep.h>

using json = nlohmann::json;

class Watchdog {
private:
    PlatformAPI& platformApi = PlatformAPI::create();
    std::string configFilePath;
    std::unordered_map<std::string, std::string> monitoredProcesses; // name -> command
    std::chrono::time_point<std::chrono::steady_clock> lastConfigCheck;

    void loadConfiguration() {
        std::ifstream file(configFilePath);
        if (!file) {
            std::cerr << "Error: Unable to read configuration file: " << configFilePath << "\n";
            return;
        }

        json config;
        file >> config;
        file.close();

        // Parse processes from configuration
        std::unordered_map<std::string, std::string> newProcesses;
        auto processes = config["processes"];
        for (const auto& process : processes) {
            std::string name = process["name"];
            std::string command = process["command"];
            newProcesses[name] = command;
        }

        // Update monitored processes
        monitoredProcesses = std::move(newProcesses);
    }

    void monitorProcesses() {
        for (const auto& [name, command] : monitoredProcesses) {
            bool isRunning = false;
            auto processes = platformApi.getRunningProcesses();

            for (const auto& proc : processes) {
                if (proc.path == command) {
                    isRunning = true;
                    break;
                }
            }

            if (!isRunning) {
                std::cout << "Process " << name << " is not running. Restarting...\n";
                platformApi.startProcess(command, {});
            }
        }
    }

    // Function to dynamically update the configuration file
    void updateConfig(const std::string &config_path, const std::string &process_name, const std::string &command_path) {
        nlohmann::json config_json;

        // Load the configuration file if it exists
        std::ifstream config_file(config_path);
        if (config_file.is_open())
        {
            try
            {
                config_file >> config_json;
            }
            catch (...)
            {
                // If JSON is invalid, start with an empty configuration
                config_json = nlohmann::json::object();
            }
        }

        // Ensure "processes" array exists in the configuration
        if (!config_json.contains("processes") || !config_json["processes"].is_array())
        {
            config_json["processes"] = nlohmann::json::array();
        }

        // Create a new process entry
        nlohmann::json new_process = {{"name", process_name}, {"command", command_path}};
        config_json["processes"].push_back(new_process);

        // Save the updated configuration
        std::ofstream updated_config_file(config_path);
        updated_config_file << config_json.dump(4); // Pretty-print with 4 spaces
    }
    // Function to allow the user to select a process
    std::vector<int> selectProcess(const std::vector<ProcessInfo> &processes) {
        std::vector<int> selected_processes;
        std::string input;

        std::cout << "Available processes:\n";
        for (size_t i = 0; i < processes.size(); ++i)
        {
            std::cout << i << ": PID=" << processes[i].pid
                    << ", Path=" << processes[i].path << "\n";
        }

        std::cout << "Enter the process numbers to select (type 'done' to finish):\n";
        while (true)
        {
            std::cout << "Process number: ";
            std::getline(std::cin, input);

            if (input == "done")
            {
                break; // Exit the loop if the user types "done"
            }

            try
            {
                int process_number = std::stoi(input);
                if (process_number >= 0 && process_number < static_cast<int>(processes.size()))
                {
                    selected_processes.push_back(process_number);
                    std::cout << "Process " << process_number << " added.\n";
                }
                else
                {
                    std::cout << "Invalid process number. Please try again.\n";
                }
            }
            catch (const std::exception &)
            {
                std::cout << "Invalid input. Please enter a number or 'done' to finish.\n";
            }
        }

        return selected_processes;
    }

public:
    Watchdog(const std::string& configPath) {
        configFilePath = configPath;
        lastConfigCheck = std::chrono::steady_clock::now();
        std::cout << "configPath: " << configPath  << std::endl;
    }

    void run() {
        // Retrieve the list of running processes
        std::vector<ProcessInfo> processes = platformApi.getRunningProcesses();

        if (processes.empty()) {
            std::cerr << "No running processes found!" << std::endl;
        }
        // Allow the user to select a process
        std::vector<int> selected = selectProcess(processes);

        std::cout << "You selected the following processes:\n";
        for (int index : selected) {
            std::cout << "PID: " << processes[index].pid
                    << ", Command Path: " << processes[index].path << "\n";
            // Use the selected process details to update the JSON configuration
            updateConfig(configFilePath, "selected_process_" + processes[index].pid, processes[index].path);
        }

        std::cout << "Configuration updated successfully!" << std::endl;

        while (true) {
            auto now = std::chrono::steady_clock::now();
            
            // Periodically reload configuration (e.g., every 2 seconds)
            if (std::chrono::duration_cast<std::chrono::seconds>(now - lastConfigCheck).count() >= 2) {
                loadConfiguration();
                lastConfigCheck = now;
            }

            // Monitor processes
            monitorProcesses();
            std::cout << "Sleep for a short duration before checking again" << std::endl;
            // Sleep for a short duration before checking again
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
    ~Watchdog() {
        delete &platformApi; // Properly delete the dynamically allocated PlatformAPI instance
    }
};
