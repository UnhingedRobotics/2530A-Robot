import math

class MotionProfiling:
    def __init__(self):
        # Motion profile parameters
        self.adjust_velocity = 6.93641618497  # Conversion from m/s to volts
        self.adjust_voltage = 0.144166666667  # Conversion from m/s to volts
        self.max_velocity = 1.73               # Max velocity (m/s)
        self.max_acceleration = 5.61           # Max acceleration (m/s^2)
        self.max_jerk = 18.1                   # Max jerk (m/s^3)
        self.deadband = 8                   # Deadband (voltage)

        # Motion profile state
        self.minimum_distance = 0              # Minimum distance for acceleration phase
        self.acceleration_time = 0             # Time to reach max velocity
        self.deadband_time = 0             # Time to reach max velocity
        self.deadband_distance = 0         # Distance covered during acceleration phase
        self.constant_distance = 0             # Distance for constant velocity
        self.constant_time = 0                 # Time for constant velocity phase
        self.total_time = 0                    # Total time for the motion profile
        self.triangular_time = 0               # Time for triangular profile
        self.time = 0                          # Current time
        self.velocity = 0                      # Current velocity
        self.decel_time = 0                    # Deceleration phase time

    def initialize(self, distance):
        """Initialize motion profile parameters for a given distance."""
        distance *= 0.0254  # Convert distance from inches to meters

        # Calculate time to reach max velocity
        self.acceleration_time = self.max_velocity / self.max_acceleration
        self.deadband_time = self.deadband * self.adjust_voltage / self.max_acceleration

        # Calculate distance covered during acceleration phase
        self.deadband_distance = 0.5 * self.max_acceleration * (self.deadband_time ** 2)
        self.acceleration_distance = 0.5 * self.max_acceleration * (self.acceleration_time ** 2)

        # Minimum distance required to reach max velocity
        self.minimum_distance =  2 * self.acceleration_distance + self.deadband_distance

        if distance >= self.minimum_distance:
            # Compute constant motion phase distance and time
            self.constant_distance = distance - (2 * self.acceleration_distance) + self.deadband_distance
            self.constant_time = self.constant_distance / self.max_velocity

            # Total time for motion profiling
            self.total_time = 2 * self.acceleration_time + self.constant_time
        else:
            # Triangular profile when distance < minimum_distance
            self.triangular_time = math.sqrt((distance + self.deadband_distance) / self.max_acceleration)
            self.total_time = 2 * self.triangular_time

    def get_velocity(self, current_time):
        """Calculate the velocity at the given time."""
        self.time = current_time
        self.velocity = 0.0  # Reset velocity

        # Determine the motion profile phase
        if self.total_time == 0:
            return 0  # No motion profile initialized

        if self.constant_distance > 0:
            # Trapezoidal profile
            if current_time < self.acceleration_time:
                # Acceleration phase
                self.velocity = self.max_acceleration * current_time
            elif current_time < self.acceleration_time + self.constant_time:
                # Constant velocity phase
                self.velocity = self.max_velocity
            elif current_time <= self.total_time:
                # Deceleration phase
                self.decel_time = self.total_time - current_time
                self.velocity = self.max_acceleration * self.decel_time
        else:
            # Triangular profile
            if current_time < self.triangular_time:
                # Acceleration phase
                self.velocity = self.max_acceleration * current_time
            elif current_time <= self.total_time:
                # Deceleration phase
                self.decel_time = self.total_time - current_time
                self.velocity = self.max_acceleration * self.decel_time

        # Adjust velocity for conversion to voltage
        self.velocity *= self.adjust_velocity
        return self.velocity
