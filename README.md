# 2530A Robot Code  

## About This Repository  

Welcome to the code repository for **Team 2530A**, where we document the development of our robot's software for the 2024–2025 VEX Robotics Competition (VRC) season. This repository contains all the code our robot will run during competitions, including templates, control systems, and experimental features.

**Note**: This uses a Linux vexcom file, to switch over to windows please check the [**vexcodetext repo**](https://github.com/UnhingedRobotics/vexcodetext)

## Features  

### Current Implementations  
- **Linear Motion Profiling with PID**:  
  A template for motion control using PID loops in conjunction with linear motion profiling.  

- **Velocity-Based Color Sorting**:  
  An advanced sorting system for a hook intake using the VEX AI Vision Sensor.  

- **Quadratic Velocity Controller**:  
  A custom velocity controller designed for improved drive performance.  

- **Odometry Integration**:  
  Adding support for precise robot position tracking (From Jar Template).

- **Boomerang Controller**
  Boomerang Controller or Algorithm that comes with Jar Template. Currently from what I have tested it doesn't work.


### Planned Features  
- **Pure Pursuit Algorithm**:  
  An initial implementation of the pure pursuit algorithm to control robot movements as well as Boomerang.

- **RAMSETE Controller or Algorithm**:
  An implementation of the RAMSETE controller for non-linear feedback control.

- **2D and Rotational Motion Profiling Feed Forward Loop**:
  An implementation of 2D Motion Profiling with custom Velocity Profiles with [Path Jerry IO](https://path.jerryio/).

- **Compilation of Mechanism Information to make custom Motion profiles**
  An implementation given weight, gear ratio, and other constants to make an acceleration and velocity motion profile over time of any mechanism.

- **Path Generation Support**:  
  Future compatibility with tools like [Path Jerry IO](https://path.jerryio/) for autonomous route creation.  

- **Custom Library Development**:  
  A reusable library for motion profiling, odometry, and advanced algorithms, potentially ported to PROS.  

---

## Development Process  

This repository has evolved over the course of the season, starting with a custom PID controller system. Below is a brief timeline:  

1. **Initial PID Controller**:  
   Created and deployed for our first tournament to control basic motion.  

2. **Exploration of Tools**:  
   Originally, we considered using [PROS](https://pros.cs.purdue.edu/), but it was challenging to set up with our environment, especially when working with the VEX AI Vision Sensor.  
   - Although there are ways to make the sensor work with PROS, we opted for VEXcode initially.  
   - On my personal account, [SerrialError](https://github.com/SerrialError), I also created a [PROS project](https://github.com/SerrialError/x-drive-v5-pros) for an X-Drive as part of earlier experiments.  

3. **Switch to JAR Template**:  
   The current project is built on top of [JAR Template](https://github.com/JAR-Template), which uses the Apache License.  
   - All relevant contributions are documented in `NOTICE.md`.  
   - A detailed version history is available in `CHANGELOG.md`.  

---

## Usage Instructions  

### Setting Up the Repository 
I made a custom repository that allows you to interact with VEX directly through the command line and through makefiles. The name is [VEX Robotics Command Line Programming Environment](https://github.com/UnhingedRobotics/vexcodetext), it is available for both Windows and Linux and more information can be found by clicking the link. Here is a list of steps on how to run it: 
1. **Download the Dependencies Listed on the [Github Page](https://github.com/UnhingedRobotics/vexcodetext)**:  
   - Download and install gcc and clang.

2. **Clone the Repository**:  
   - Clone or download this repository to your computer.  

3. **Change Any Software Modifications to Your Specific Hardware Design**:  
   - Change or get rid of the intake and/or fishmech.

4. **Download SDK**:
   - Run
   ```bash
   make
   ```
   in your terminal

5. **Deploy the Code**:  
   - Run
   ```bash
   make clean upload
   ```
   in your terminal

### Notes on Customization  
- **Hardware Adjustments**:  
  - You may need to modify the motor configurations to match your robot. The [JAR Template Documentation](https://github.com/JAR-Template/JAR-Template) provides instructions on adding, removing, and defining motors.  

- **Constants**:  
  - If using specific features (e.g., color sorting), you must update the constants to fit your robot's setup.  
  - Most constants are self-explanatory, but feel free to reach out with questions if needed.  

- **Autonomous Routes**:  
  - The current repository includes our autonomous routes, but these will need to be customized for your robot's field setup and objectives.  

---

## Future Plans  

In the future, I plan to:  
1. Develop a standalone library to simplify integration of motion profiling, odometry, and path planning.  
2. Add support for autonomous path generation via tools like [Path Jerry IO](https://path.jerryio/).  
3. Transition back to PROS for advanced customization, including seamless integration with the VEX AI Vision Sensor.  

---

## License  

This repository is built on [JAR Template](https://github.com/JAR-Template/JAR-Template), which is licensed under the Apache License 2.0. All changes and contributions are documented in `NOTICE.md` and `CHANGELOG.md`.  

For questions, issues, or collaboration, feel free to open an issue or contact me directly!  

---
=======
# VEX Robotics Command Line Programming Environment  

This repository provides a lightweight, text-based environment for programming and building VEX robotics projects. It is designed as an alternative to the VEX VSCode Extension, allowing you to upload, build, and manage your VEX projects without the need for a full IDE.  

**Note**: This project is **not affiliated with VEX Robotics**. It utilizes files extracted from the official VEX VSCode extension and VEXcode application to enable functionality.

**Note**: Great inspiration was taken from this [**article on Medium**](https://medium.com/@sw_47875/setting-up-command-line-c-tools-for-your-vex-v5-team-283277c8d79c).

**Note**: This is the [**Linux Branch**](https://github.com/UnhingedRobotics/vexcodetext/tree/linux) if you use windows please go the [**Main Branch**](https://github.com/UnhingedRobotics/vexcodetext).

---

## Features  
- Cross-platform support: Available for [**Windows**](https://github.com/UnhingedRobotics/vexcodetext) and [**Linux**](https://github.com/UnhingedRobotics/vexcodetext/tree/linux).  
- Fully text-based interface for streamlined workflows.  
- Supports building and uploading VEX Robotics programs using standard CLI tools.  

---

## Dependencies and Initial Necessary Steps for Linux 

To build and upload your VEX programs, you need the following tools installed on your system:  
- **clang**  
- **arm-none-eabi-objcopy**  
- **arm-none-eabi-size**  
- **arm-none-eabi-ld**  
- **arm-none-eabi-ar**  

### Installing Dependencies  

#### **Linux**  
1. Install the **ARM GCC toolchain**:  
   - On Debian-based systems (e.g., Ubuntu):  
     ```bash
     sudo apt update  
     sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi  
     ```  
   - On Arch Linux:  
     ```bash
     sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils  
     ```  
2. Install **clang**:  
   - On Debian-based systems:  
     ```bash
     sudo apt install clang  
     ```  
   - On Arch Linux:  
     ```bash
     sudo pacman -S clang  
     ```  


### **Configuring Serial Connections with udev Rules**

To enable proper communication with the VEX Robotics hardware over a serial connection, you need to configure udev rules. This ensures that the device (e.g., /dev/ttyACM0) has the correct permissions for access.

#### Steps to Configure udev Rules:

1. Create a new udev rules file in the /etc/udev/rules.d/ directory:
    ```bash
    sudo nano /etc/udev/rules.d/01-vex-v5.rules
    ```
2. Add the following line to the file to set permissions for VEX devices:

    ```bash
    KERNEL=="ttyACM[0-9]*", MODE="0666", ATTRS{idVendor}=="2888", ATTRS{idProduct}=="0503"
    ```

   - **Explanation**:
    - `KERNEL=="ttyACM[0-9]*"`: Matches any device with the name pattern `/dev/ttyACM*`.
    - `MODE="0666"`: Sets read and write permissions for all users.
    - `ATTRS{idVendor}=="2888"`: Matches the VEX Robotics Vendor ID.
    - `ATTRS{idProduct}=="0503"`: Matches the VEX Robotics V5 Controller Product ID.

3. Save and close the file.  
   - In `nano`, press `Ctrl+O` to save, then `Enter` to confirm, and `Ctrl+X` to exit.

4. Reload the `udev` rules:

    ```bash
    sudo udevadm control --reload-rules
    ```
5. Unplug and reconnect the VEX Robotics device.

6. Verify the device is accessible:

    ```bash
    ls /dev/ttyACM*
    ```
#### Troubleshooting

- If the device is not recognized or permissions are incorrect, double-check the Vendor ID and Product ID using the lsusb command:
  ``` bash
  lsusb
  ```

Look for a line like:

  ``` bash
  Bus 003 Device 015: ID 2888:0503 VEX Robotics, Inc VEX Robotics V5 Controller
  ```

- Update the idVendor and idProduct values in the udev rules file if necessary.

---

## Making your project

1. Make a new folder named src
2. Put any cpp files you want to run in it (typically main.cpp)

## Running the project

- First to download the sdk run
```bash
make
```
- Then for building run 
```bash
make clean build
```
- And for uploading run
```bash
make clean upload
```
