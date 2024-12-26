# Cooling-Loop
A cooling loop for an inverter and DC-DC; has the following components, 1. Brushless Water Pump 2. Filter 3. Radiator with Fan 4. Temperature Sensor 5. Inverter 6. DC-DC Converter 7. Open Reservoir 8. Level Switch 9. Orifice
This is a mobile application. There is no right answer as there are multiple ways the system can be configured.
Create the electrical schematic with the program of your choice. Straight-edge hand-drawn is also acceptable.
The list of components you should use is below. You can also use diodes, relays, and fuse or circuit breakers
where appropriate.
1. Two 12V lead acid batteries
2. F-Series Single Pole Toggle Switch – For system Ignition
3. IFM PLC CR0403
4. EMP Pump WP32
5. Power View Screen – Used to view temperature and pump status
6. H-WTMS Temperature Sensor
7. VA97-BBL339P/N-103A Fan
8. LMC100 Liquid Level Switch
## Coding
Write the logic for the cooling loop in Challenge 1 that would run on the PLC to control the coolant
temperature so that the Inverter and DC-DC can operate at max power without derating. You should consider
the following.
1. Temperature Sensor Input
2. Ignition Switch Input
3. Controlling the Pump
4. Controlling the Fan
5. Applicable Safety Functions

![image](https://github.com/user-attachments/assets/f0b08e21-385b-4562-ab91-0bba623cbf3a)

## Instructions:
1. Write meaningful code comments.
2. Proper indentation.
3. Use variables with emulated data to demonstrate your program.
4. Zip your program folder on completion and attach it to your submissions
The methods below used: C/C++ Code using a GCC compiler and MSYS2 shell. Use fake variable values to demonstrate working
logic. Use the links provided to set up a dev environment (MSYS2)
C++ programming with Visual Studio Code.
## Firmware
1. Simulate sending and receiving data over CANBUS.
2. Use a PID loop.
3. Create a state machine.
4. Pass command line arguments for setpoints.
5. Use external dependencies managed by CMake or Vcpkg.
6. Build on Linux, use a shell script to launch and pass params at runtime or use MSYS2 and python.
7. Use Gtest to implement unit testing.
8. “DO NOT” ship dependencies in your project, use static linking or use your build system.

### Compile & Run
To rebuild the project with the source file located at /workspaces/Cooling-Loop/CoolingLoopControl_V1.1/src/CoolingLoopControl_V1.1.cpp, follow these steps:
Steps to Build

    Navigate to Your Workspace: Open the terminal in GitHub Codespaces and move to your project directory:

cd /workspaces/Cooling-Loop/CoolingLoopControl_V1.1

Clean the Previous Build: Remove the old build directory if it exists to start fresh:

rm -rf build

Create a Build Directory: Create a new directory for build files:

mkdir build
cd build

Run CMake: Generate the build system:

cmake ..

Compile the Project: Build the project using the generated Makefiles:

make

Run the Built Executables: After the build completes, run the executables:

    Main Application:

./CoolingLoopControl

Unit Tests:

./CoolingLoopControlTest
