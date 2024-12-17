# 2530A Robot Code
## What is this

We are team 2530A and this is a code repository of the code our robot will be running over the course of the 2024-2025 season for V5RC.

## What code does this include?
There is a template for only linear motion profiling currently that is used in conjunction with PID. Also there is velocity color sorting for a hook intake that uses an AI sensor. Also there is a quadratic velocity controller for driving. Also I have a basic implimentation of pure pursuit that hasn't been tested.

## How did you make this

Initially this started as a custom PID controller system for our robot. My plan was to eventually add odometry and pure pursuit.
I wanted to use PROS to start, however it was annoying setting it up with my computer also I wanted to use the AI Vision Sensor which currently is only directly supported through vexcode (However there are janky ways to get it working through PROS which I now know). So I decided to start with vexcode (However before the start of the season on my personal account, SerrialError, I made a PROS project for an X-Drive).
I implimented the custom PID controller for our first tournement. Then I switched over to JAR Template. All of the contributions and other necessary things having to do with JAR Template can be found in NOTICE.md.
JAR Template uses the apache license, so in order to comply I have a CHANGELOG.md. Here you can see all of our versions where I will eventually put in the versions tab so you can download.

## How can I use this

You will need to open a new vscode project with the VEX vscode plugin. Then download this repository. Then copy over the vex volder into this repository. Then open it with vscode. Then you should be able to download to your brain using the VEX vscode plugin.
Currently this is not a library, it is just a code repository for our robot. However eventually I am planning to make my own library, possibly with PROS.
Currently also if you want to use this you will most likely have to get rid of a lot of the motors. JAR Template explains how to define, add, and remove motors on their website.
If you do want to use color sorting or any other custom function I am using then you will need to change the constants. Most should be self explanatory, however if you have any questions feel free to ask.
Again as I am using this for my personal robot there is my autonomous routes. In the future I also want to add a way for autons to be made through path.jerryio if I make a library.
