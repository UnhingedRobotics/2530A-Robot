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

- **Basic Pure Pursuit Algorithm**:  
  An initial implementation of pure pursuit for path following (untested as of now).  

### Planned Features  
- **Odometry Integration**:  
  Adding support for precise robot position tracking.  

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
   - Change or get rid of the intake and fishmech.

4. **Deploy the Code**:  
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
