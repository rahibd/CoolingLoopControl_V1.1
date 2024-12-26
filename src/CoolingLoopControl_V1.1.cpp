/*
Cooling Loop Control System for Inverter and DC-DC Converter with PID Control
Author: Mozammel Bin Motalab
Date: 10/12/2024

Scenario & Assumptions:
This project implements a cooling loop control system designed to manage the thermal conditions of two critical components:
the Inverter and the DC-DC Converter. The system ensures these components operate at maximum power without overheating or
derating, enhancing performance and reliability.

CAN Bus Communication:
Sends the operational status of the pump and fan over a CAN Bus network.
Encodes messages with a predefined CAN ID 18FF408F such that, motor can be controlled by this.

Future Enhancements:
    Replace simulated sensor data with actual hardware inputs.
    Implement a logging system for debugging and maintenance.
    Add dynamic user inputs for thresholds via command-line arguments or configuration files.
    Incorporate advanced error handling for CAN Bus communication.
*/

#include <iostream>
#include <thread> // For simulating delays
#include <cstdlib> // For emulated data
#include <cmath> // For PID calculations
#include <string> // For command-line argument handling
#include <sstream> // For parsing arguments

// PID Controller class
class PIDController {
private:
    float Kp, Ki, Kd;
    float prevError, integral;

public:
    PIDController(float p, float i, float d) : Kp(p), Ki(i), Kd(d), prevError(0.0), integral(0.0) {}

    float compute(float setpoint, float measuredValue) {
        float error = setpoint - measuredValue;
        integral += error;
        float derivative = error - prevError;
        prevError = error;
        return (Kp * error) + (Ki * integral) + (Kd * derivative);
    }
};

// State machine states
enum class SystemState {
    OFF,
    ON,
    SAFETY_SHUTDOWN
};

// Functions
void controlPump(float speed);
void controlFan(float speed);
void safetyShutdown(SystemState& state);
void CANcontrol(float pumpSpeed, float fanSpeed);
float interpolateTemperature(float voltage);

#ifndef UNIT_TEST
int main(int argc, char* argv[]) {
    // Parse command-line arguments for setpoints
    float tempSetpoint = 50.0; // Default setpoint
    float safetyThreshold = 70.0; // Default safety threshold

    if (argc > 1) {
        try {
            tempSetpoint = std::stof(argv[1]);
            if (argc > 2) {
                safetyThreshold = std::stof(argv[2]);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing command-line arguments: " << e.what() << "\n";
            return 1;
        }
    }

    // PID Controllers
    PIDController pumpPID(0.5f, 0.1f, 0.05f); // Tuned values for pump
    PIDController fanPID(0.4f, 0.1f, 0.03f);  // Tuned values for fan

    // Emulated sensor data (replace with real inputs in actual implementation)
    bool ignitionSwitch = false; // Ignition switch input
    float sensorVoltage = 0.0; // Simulated voltage reading
    bool levelSwitch = true; // Coolant level (true = sufficient, false = low)
    float measuredTemperature = 0.0; // Actual temperature

    // Declare pumpSpeed and fanSpeed outside the loop to ensure they are accessible globally
    float pumpSpeed = 0.0;
    float fanSpeed = 0.0;

    // Initialize system
    SystemState currentState = SystemState::OFF;
    std::cout << "Initializing cooling loop with PID control..." << std::endl;

    // Main control loop
    while (true) {
        switch (currentState) {
            case SystemState::OFF:
                ignitionSwitch = !ignitionSwitch; // Simulate ignition switch toggle
                if (ignitionSwitch) {
                    std::cout << "System ON\n";
                    currentState = SystemState::ON;
                } else {
                    std::cout << "System remains OFF\n";
                }
                break;

            case SystemState::ON:
                // Simulate sensor voltage readings (replace with real sensor inputs)
                sensorVoltage = 1.0f + static_cast<float>(rand() % 3); // Random voltage between 1.0-4.0V

                // Interpolate temperature from voltage
                measuredTemperature = interpolateTemperature(sensorVoltage);

                // Check coolant level
                if (!levelSwitch) {
                    std::cerr << "ERROR: Low coolant level. Shutting down pump and fan for safety.\n";
                    controlPump(0);
                    controlFan(0);
                    safetyShutdown(currentState);
                    break;
                }

                // Compute PID outputs for pump and fan
                pumpSpeed = pumpPID.compute(tempSetpoint, measuredTemperature);
                fanSpeed = fanPID.compute(tempSetpoint, measuredTemperature);

                // Outputs to valid ranges (0-100%)
                if (pumpSpeed < 0.0f) pumpSpeed = 0.0f;
                if (pumpSpeed > 100.0f) pumpSpeed = 100.0f;

                if (fanSpeed < 0.0f) fanSpeed = 0.0f;
                if (fanSpeed > 100.0f) fanSpeed = 100.0f;

                // Apply control outputs
                controlPump(pumpSpeed);
                controlFan(fanSpeed);

                // Safety shutdown if temperature exceeds critical threshold
                if (measuredTemperature > safetyThreshold) {
                    std::cerr << "\033[31mCRITICAL: Overtemperature detected. Shutting down system.\033[0m\n";
                    controlPump(0);
                    controlFan(0);
                    safetyShutdown(currentState);
                    break;
                }

                // Display status
                std::cout << "Measured Temperature: " << measuredTemperature << "°C\n";
                std::cout << "Pump Speed: " << pumpSpeed << "%\n";
                std::cout << "Fan Speed: " << fanSpeed << "%\n";
                break;

            case SystemState::SAFETY_SHUTDOWN:
                std::cerr << "System in SAFETY SHUTDOWN mode. Please restart the system.\n";
                return 0;
        }

        // Simulate delay (replace with real-time loop in PLC or embedded system)
        std::this_thread::sleep_for(std::chrono::seconds(1));
        CANcontrol(pumpSpeed, fanSpeed);
    }

    return 0;
}
#endif
// Function to control the pump
void controlPump(float speed) {
    std::cout << "Pump running at " << speed << "% speed.\n";
}

// Function to control the fan
void controlFan(float speed) {
    std::cout << "Fan running at " << speed << "% speed.\n";
}

// Function for safety shutdown
void safetyShutdown(SystemState& state) {
    state = SystemState::SAFETY_SHUTDOWN;
    std::cerr << "System entering safety shutdown mode.\n";
}

// Simulate CAN Bus control messages
void CANcontrol(float pumpSpeed, float fanSpeed) {
    const int CANID = 0x18FF408F; // CAN ID for the message
    const int DLC = 8; // Data Length Code
    unsigned char msg[DLC] = {0};

    // Encode speeds into CAN message
    msg[2] = static_cast<unsigned char>(pumpSpeed / 100 * 255); // Scale pump speed to 0-255
    msg[6] = static_cast<unsigned char>(fanSpeed / 100 * 255);  // Scale fan speed to 0-255

    // Print CAN message
    std::cout << "CANID: 0x" << std::hex << std::uppercase << CANID << "\n";
    std::cout << "MSG: ";
    for (int i = 0; i < DLC; ++i) {
        std::cout << "0x" << std::hex << std::uppercase << static_cast<int>(msg[i]) << " ";
    }
    std::cout << "\n";
}

// Interpolate temperature from voltage based on sensor data table
float interpolateTemperature(float voltage) {
    // Example mapping based on the provided table (simplified linear interpolation)
    if (voltage >= 4.771) return -20.0;
    if (voltage >= 4.642) return -10.0;
    if (voltage >= 4.438) return 0.0;
    if (voltage >= 4.141) return 10.0;
    if (voltage >= 3.751) return 20.0;
    if (voltage >= 3.325) return 30.0;
    if (voltage >= 2.838) return 40.0;
    if (voltage >= 2.500) return 50.0;
    if (voltage >= 1.915) return 60.0;
    if (voltage >= 1.212) return 80.0;
    if (voltage >= 0.749) return 100.0;
    return 120.0; // Default for lower voltages
}