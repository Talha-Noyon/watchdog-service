#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <iostream>
#include <psapi.h>
#include "watchdog/windows_api.h"

std::vector<ProcessInfo> WindowsAPI::getRunningProcesses() {

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

bool WindowsAPI::startProcess(const std::string& command, const std::vector<std::string>& args) {
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

#endif