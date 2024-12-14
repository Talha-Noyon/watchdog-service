# Watchdog Service

A lightweight, cross-platform watchdog service written in C++ to monitor and manage processes/applications dynamically. The service ensures configured processes are running and automatically restarts them if they stop or are terminated. Additionally, the configuration is dynamically reloaded when changes are made to the configuration file.

---

## Features

- **Dynamic Configuration Reloading**: Automatically reloads the configuration when the configuration file is updated.
- **Process Monitoring**: Ensures all specified processes are running, restarting them if necessary.
- **Cross-Platform Support**: Compatible with Linux and Windows, using platform-specific APIs encapsulated in a unified interface.
- **Encapsulation of Native OS API**: Native OS API details are encapsulated in a separate component to ensure a unified interface and extensibility.
- **Extensible Architecture**: Modular design for easily adding other operating systems or features.
- **Comprehensive Logging**: Logs all significant events and actions for review and analysis.
- **Periodic Monitoring**: Supports periodic checks for monitoring processes.
- **Testing Coverage**: Comprehensive unit tests for components and integration tests for workflows.
---

## Configuration File

The configuration file uses JSON format to specify processes and their commands:

```json
{
    "processes": [
        {
            "name": "Process1",
            "command": "/path/to/command1"
        },
        {
            "name": "Process2",
            "command": "/path/to/command2"
        }
    ]
}
```
---

## Getting Started

### Prerequisites

- **Linux**: Visual Studio code, GCC 9.1+ or Clang 9.0+ with `std::filesystem` support.
- **Windows**: [Visual Studio code with minGW (C++ Compiler)](https://www.youtube.com/watch?v=DMWD7wfhgNY).
- **CMake**: Version 3.15 or higher.

### Installation
1. Install make for windows
   using [chocolatey](https://docs.chocolatey.org/en-us/choco/setup/#install-with-powershellexe)
   ```bash
   choco install make
   ```
2. install make for Ubuntu/Debian-based Systems:
   - Update the package list:
     ```bash
     sudo apt update
     sudo apt install -y make


3. Clone the repository:
    ```bash
    git clone https://github.com/Talha-Noyon/watchdog-service.git
    cd watchdog-service
    ```

4. Build the project using git bash:
    ```bash
    rm -rf build && mkdir build && cd build
    cmake ..
    make
    ```

5. Run the application:
    ```bash
    ./WatchdogApp
    ```

---

## Contact

- **Author**: Md Abu Talha 
- **Email**: talha.noyon10@gmail.com  
- **GitHub**: [Talha-Noyon](https://github.com/Talha-Noyon)
