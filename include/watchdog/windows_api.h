#ifndef WINDOWS_API_H
#define WINDOWS_API_H

#include "watchdog/platform_api.h"
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <iostream>
#include <psapi.h>

class WindowsAPI : public PlatformAPI {
    public:
        // Retrieves a list of running processes on the system.
        std::vector<ProcessInfo> getRunningProcesses() override {
            std::vector<ProcessInfo> processes;

            HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hProcessSnap == INVALID_HANDLE_VALUE) {
                std::cerr << "Failed to take process snapshot.\n";
                return processes;
            }

            PROCESSENTRY32 pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(hProcessSnap, &pe32)) {
                do {
                    std::string process_name = pe32.szExeFile;
                    DWORD process_id = pe32.th32ProcessID;

                    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
                    if (hProcess) {
                        char process_path[MAX_PATH];
                        if (GetModuleFileNameEx(hProcess, nullptr, process_path, MAX_PATH)) {
                            processes.push_back({std::to_string(process_id), process_path});
                        }
                        CloseHandle(hProcess);
                    }
                } while (Process32Next(hProcessSnap, &pe32));
            }

            CloseHandle(hProcessSnap);
            return processes;
        }

        // Starts a process with the given command and arguments.
        bool startProcess(const std::string& command, const std::vector<std::string>& args) override {
            std::string cmd = command;
            for (const auto& arg : args) {
                cmd += " " + arg;
            }

            STARTUPINFO si = { sizeof(STARTUPINFO) };
            PROCESS_INFORMATION pi;
            if (CreateProcess(NULL, const_cast<char*>(cmd.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return true;
            }
            return false;
        }

        // Stops a process with the given PID.
        bool stopProcess(const std::string& pid) override {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, std::stoi(pid));
            if (hProcess) {
                bool success = TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
                return success;
            }
            return false;
        }

        // Checks if a process with the given PID is running.
        bool isProcessRunning(const std::string& pid) override {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, std::stoi(pid));
            if (hProcess) {
                CloseHandle(hProcess);
                return true;
            }
            return false;
        }
        
    private:
        WindowsAPI() {} // Private constructor to prevent direct instantiation

        friend PlatformAPI& PlatformAPI::create(); // Allow `PlatformAPI` to create an instance
};

#endif // WINDOWS_API_H
