import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import random
import time
import numpy as np

# Function to update the robot's position, heading, and velocity
def update_position(x, y, heading, velocity):
    plt.clf()  # Clear the figure to update the plot
    
    # Load and display the field image as background
    img = mpimg.imread("field_image.png")  # Load the background image
    plt.imshow(img, extent=[0, 12, 0, 12])  # Set the field boundaries to 12x12 feet

    # Plot the robot position as a red dot
    plt.plot(x, y, 'ro', label="Robot Position")
    
    # Calculate the arrow's x and y components based on the heading (angle)
    arrow_length = velocity  # Length of the arrow proportional to velocity (scale factor of 0.1)
    dx = arrow_length * np.cos(np.radians(heading))  # x component of the direction
    dy = arrow_length * np.sin(np.radians(heading))  # y component of the direction
    
    # Draw the arrow (representing velocity direction)
    plt.quiver(x, y, dx, dy, angles='xy', scale_units='xy', scale=1, color='b', width=0.005)
    
    # Add labels and a title
    plt.xlabel("X Position (feet)")
    plt.ylabel("Y Position (feet)")
    plt.title("Robot Position, Heading, and Velocity on the Field")
    
    # Optionally, display a grid
    plt.grid(True)
    
    # Show the plot
    plt.legend()
    plt.draw()
    plt.pause(0.1)  # Pause to allow the plot to update

# Example usage: update the plot in a loop with random robot positions, headings, and velocities
plt.ion()  # Turn on interactive mode

# Infinite loop to keep the plot updating with random positions, headings, and velocities every second
while True:
    # Generate random positions for the robot within the field (12x12 feet)
    x = random.uniform(0, 12)  # Random x position between 0 and 12 feet
    y = random.uniform(0, 12)  # Random y position between 0 and 12 feet
    
    # Generate a random heading (angle) between 0 and 360 degrees
    heading = random.uniform(0, 360)
    
    # Generate a random velocity between 0 and 5 ft/s (adjust range as needed)
    velocity = random.uniform(0, 5)  # Robot's velocity in ft/s
    
    # Update the robot's position, heading, and velocity on the plot
    update_position(x, y, heading, velocity)
    
    # Wait for 1 second before updating the position, heading, and velocity
    time.sleep(1)

# Keeps the plot open and running indefinitely
plt.show(block=True)
