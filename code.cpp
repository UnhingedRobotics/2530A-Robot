#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

// Function to generate a random float in a given range
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int main() {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Create a window with size 600x600 (scaled down for 12x12 ft field)
    sf::RenderWindow window(sf::VideoMode(600, 600), "Robot Position, Heading, and Velocity");

    // Set the frame rate limit to 60 FPS
    window.setFramerateLimit(60);

    // Load the field image (ensure "field_image.png" is in the working directory)
    sf::Texture fieldTexture;
    if (!fieldTexture.loadFromFile("field_image.png")) {
        std::cerr << "Error loading field image!\n";
        return -1;
    }
    sf::Sprite fieldSprite(fieldTexture);
    fieldSprite.setScale(600.0f / fieldTexture.getSize().x, 600.0f / fieldTexture.getSize().y); // Scale image to fit window

    // Main loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Generate random robot position (scaled for 12x12 ft field)
        float x = randomFloat(0, 12); // Random x position in feet
        float y = randomFloat(0, 12); // Random y position in feet

        // Convert feet to pixels (50 pixels per foot)
        float xPixels = x * 50;
        float yPixels = y * 50;

        // Generate random heading (angle in degrees)
        float heading = randomFloat(0, 360);

        // Generate random velocity in ft/s
        float velocity = randomFloat(0, 5); // Velocity in feet per second

        // Calculate arrow length based on velocity (scale factor = 10 pixels per ft/s)
        float arrowLength = velocity * 10;

        // Calculate arrow direction components (dx, dy) using the heading
        float dx = arrowLength * std::cos(heading * 3.14159265358979 / 180.0f); // Convert degrees to radians
        float dy = arrowLength * std::sin(heading * 3.14159265358979 / 180.0f);

        // Clear the window
        window.clear();

        // Draw the field
        window.draw(fieldSprite);

        // Draw the robot as a red circle
        sf::CircleShape robot(5); // Robot radius = 5 pixels
        robot.setFillColor(sf::Color::Red);
        robot.setPosition(xPixels - robot.getRadius(), yPixels - robot.getRadius()); // Center the circle on position
        window.draw(robot);

        // Draw the arrow indicating the heading
        sf::Vertex arrow[] = {
            sf::Vertex(sf::Vector2f(xPixels, yPixels), sf::Color::Blue),
            sf::Vertex(sf::Vector2f(xPixels + dx, yPixels + dy), sf::Color::Blue)
        };
        window.draw(arrow, 2, sf::Lines);

        // Display everything
        window.display();

        // Wait for 1 second before updating
        sf::sleep(sf::seconds(1));
    }

    return 0;
}
