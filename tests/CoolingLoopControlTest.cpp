#define UNIT_TEST
#include <gtest/gtest.h>
#include "../src/CoolingLoopControl_V1.1.cpp"

// Test for PIDController
TEST(PIDControllerTest, Compute) {
    PIDController pid(1.0, 0.1f, 0.01f); // Test values
    EXPECT_NEAR(pid.compute(50.0, 45.0), 5.5, 0.1);  // Example test
    EXPECT_NEAR(pid.compute(50.0, 50.0), 0.0, 0.1);  // Edge case
}

// Test for interpolateTemperature
TEST(InterpolateTemperatureTest, VoltageToTemperature) {
    EXPECT_EQ(interpolateTemperature(4.771f), -20.0);
    EXPECT_EQ(interpolateTemperature(2.838f), 40.0);
}

// Test for controlPump
TEST(ControlPumpTest, SpeedOutput) {
    testing::internal::CaptureStdout();
    controlPump(50.0);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Pump running at 50% speed.\n");
}
