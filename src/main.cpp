#include "watchdog/platform_api.h"
#include "watchdog/watchdog.h"

int main(int argc, char* argv[]) {
    /* if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config-file-path>\n";
        return 1;
    }

    const std::string configFilePath = argv[1]; */
    const std::string configFilePath = "data/watchdog.json";
    // Create and run the watchdog
    Watchdog watchdog(configFilePath);
    watchdog.initializeFromSelection();
    watchdog.run();
    return 0;
}
