#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "watchdog/watchdog.h"
#include "watchdog/platform_api.h"
#include <fstream>

const std::string& configPath = "data/watchdog.json";
// Mock PlatformAPI to provide mock implementations for the virtual methods
class MockPlatformAPI : public PlatformAPI {
  public:
      MOCK_METHOD(std::vector<ProcessInfo>, getRunningProcesses, (), (override));
      MOCK_METHOD(bool, startProcess, (const std::string& command, const std::vector<std::string>& args), (override));
};

// Test case
class WatchdogTest : public ::testing::Test {
  protected:
      MockPlatformAPI mockPlatformAPI;
};

TEST_F(WatchdogTest, TestStartProcess) {
    // Use a std::string for the command and std::vector<std::string> for arguments
    std::string command = "C:\\Program Files (x86)\\WinSCP\\WinSCP.exe";
    std::vector<std::string> args = {};  // Empty arguments for this example

    // Set expectations on the mock
    EXPECT_CALL(mockPlatformAPI, startProcess(command, args))
        .Times(1)
        .WillOnce(::testing::Return(true));  // Simulate a successful process start

    // Trigger the mock
    bool result = mockPlatformAPI.startProcess(command, args);
    EXPECT_TRUE(result);
}

TEST_F(WatchdogTest, TestGetRunningProcesses) {
    // Example of mocking the `getRunningProcesses` method
    std::vector<ProcessInfo> mockProcesses = {ProcessInfo{"1234","C:\\Program Files (x86)\\WinSCP\\WinSCP.exe"}};
    EXPECT_CALL(mockPlatformAPI, getRunningProcesses())
        .WillOnce(::testing::Return(mockProcesses));

    // Trigger the mock
    std::vector<ProcessInfo> processes = mockPlatformAPI.getRunningProcesses();
    EXPECT_EQ(processes.size(), 1);
    EXPECT_EQ(processes[0].path, "C:\\Program Files (x86)\\WinSCP\\WinSCP.exe");
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
