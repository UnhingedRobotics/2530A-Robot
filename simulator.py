import time
import matplotlib.pyplot as plt
from motion_profiling import MotionProfiling

def simulate_motion_profile(distance):
    # Initialize motion profiling
    mp = MotionProfiling()
    mp.initialize(distance)

    # Simulation parameters
    time_step = 0.01  # Time step (s)
    total_time = mp.total_time
    current_time = 0
    velocities = []
    positions = []
    times = []

    # Initial position
    position = 0

    # Simulation loop
    while current_time <= total_time:
        # Get current velocity
        velocity = mp.get_velocity(current_time)
        velocities.append(velocity)

        # Update position
        position += (velocity / mp.adjust_velocity) * time_step  # Convert back to m/s
        positions.append(position)

        # Update time
        times.append(current_time)
        current_time += time_step

        # Simulate time delay (optional, for real-time feel)
        time.sleep(time_step)

    # Plot results
    plt.figure(figsize=(10, 6))
    plt.subplot(2, 1, 1)
    plt.plot(times, velocities, label="Velocity (m/s)")
    plt.xlabel("Time (s)")
    plt.ylabel("Velocity (m/s)")
    plt.title("Velocity vs Time")
    plt.grid()
    plt.legend()

    plt.subplot(2, 1, 2)
    plt.plot(times, positions, label="Position (m)", color="orange")
    plt.xlabel("Time (s)")
    plt.ylabel("Position (m)")
    plt.title("Position vs Time")
    plt.grid()
    plt.legend()

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # Simulate motion profile for a distance of 100 inches
    simulate_motion_profile(distance=10)
