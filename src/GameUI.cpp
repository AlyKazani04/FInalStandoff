#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text title;
    sf::RectangleShape playButton;
    sf::RectangleShape exitButton;
    sf::Text playText;
    sf::Text exitText;
    sf::Sprite titleBox;
    sf::Texture titleBoxTexture;
    sf::Texture circleTexture;
    sf::Sprite leftCircle;
    sf::Sprite rightCircle;
    bool isMenuOpen;

public:
    Menu(sf::RenderWindow& window) : window(window), isMenuOpen(true),
        title(font),
        playText(font),
        exitText(font),
        titleBox(titleBoxTexture),
        leftCircle(circleTexture),
        rightCircle(circleTexture) {
        // Load font
        if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cout << "Error loading font" << std::endl;
            return;
        }

        // Load title box texture
        if (!titleBoxTexture.loadFromFile("/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/UI/PIC.jpeg")) {
            std::cout << "Error loading title box texture" << std::endl;
            return;
        } else {
            std::cout << "Successfully loaded title box texture. Size: " << titleBoxTexture.getSize().x << "x" << titleBoxTexture.getSize().y << std::endl;
        }

        // Load circle texture
        if (!circleTexture.loadFromFile("/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/UI/PIC.jpeg")) {
            std::cout << "Error loading circle texture" << std::endl;
            return;
        } else {
            std::cout << "Successfully loaded circle texture. Size: " << circleTexture.getSize().x << "x" << circleTexture.getSize().y << std::endl;
        }

        // Setup play button
        playButton.setSize(sf::Vector2f(200.0f, 50.0f));
        playButton.setPosition(sf::Vector2f(
            window.getSize().x / 2.0f - 100.0f,
            300.0f
        ));
        playButton.setFillColor(sf::Color::Green);

        // Setup exit button
        exitButton.setSize(sf::Vector2f(200.0f, 50.0f));
        exitButton.setPosition(sf::Vector2f(
            window.getSize().x / 2.0f - 100.0f,
            400.0f
        ));
        exitButton.setFillColor(sf::Color::Red);

        // Center both title and title box
        float titleWidth = title.getLocalBounds().size.x;
        float titleBoxWidth = titleBox.getLocalBounds().size.x * 6.0f; // Account for scale

        // Setup title box
        titleBox.setTexture(titleBoxTexture);
        titleBox.setScale(sf::Vector2f(6.0f, 4.0f));
        titleBox.setPosition(sf::Vector2f(
            window.getSize().x / 2.0f - (titleBoxWidth / 2),
            80  // Slightly above the title
        ));

        // Setup title
        title.setString("Final StandOff");
        title.setCharacterSize(64);
        title.setFillColor(sf::Color::Black);
        
        title.setPosition(sf::Vector2f(
            window.getSize().x / 2.0f - titleWidth / 2,
            100
        ));
        
        // Setup circles on both sides of the title
        float circleScale = 3.0f; // Increased scale
        leftCircle.setScale(sf::Vector2f(circleScale, circleScale));
        rightCircle.setScale(sf::Vector2f(circleScale, circleScale));
        
        // Position circles relative to title
        float circleWidth = circleTexture.getSize().x * circleScale;
        float spacing = 40.0f; // Increased spacing
        
        leftCircle.setPosition(sf::Vector2f(
            window.getSize().x / 2.0f - titleWidth / 2 - circleWidth - spacing,
            100 + title.getCharacterSize() / 2 - circleWidth / 2
        ));
        
        rightCircle.setPosition(sf::Vector2f(
            window.getSize().x / 2.0f + titleWidth / 2 + spacing,
            100 + title.getCharacterSize() / 2 - circleWidth / 2
        ));

        // Add origin setting to center the circles
        leftCircle.setOrigin(sf::Vector2f(circleTexture.getSize().x / 2.0f, circleTexture.getSize().y / 2.0f));
        rightCircle.setOrigin(sf::Vector2f(circleTexture.getSize().x / 2.0f, circleTexture.getSize().y / 2.0f));

        // Setup text for buttons
        playText.setString("Play");
        playText.setCharacterSize(30);
        playText.setFillColor(sf::Color::Black);
        playText.setPosition(sf::Vector2f(
            playButton.getPosition().x + (200.0f - playText.getLocalBounds().size.x) / 2,
            playButton.getPosition().y + (50.0f - playText.getCharacterSize()) / 2
        ));

        exitText.setString("Exit");
        exitText.setCharacterSize(30);
        exitText.setFillColor(sf::Color::Black);
        exitText.setPosition(sf::Vector2f(
            exitButton.getPosition().x + (200.0f - exitText.getLocalBounds().size.x) / 2,
            exitButton.getPosition().y + (50.0f - exitText.getCharacterSize()) / 2
        ));
    }

    void processEvents() {
        while(const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (playButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    // Handle play button click
                    std::cout << "Play clicked!" << std::endl;
                }
                else if (exitButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    window.close();
                }
            }
            // Add hover effects
            else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (playButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    playButton.setFillColor(sf::Color(200, 200, 200));  // Slightly darker when hovered
                } else {
                    playButton.setFillColor(sf::Color::Green);
                }
                if (exitButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    exitButton.setFillColor(sf::Color(200, 200, 200));
                } else {
                    exitButton.setFillColor(sf::Color::Red);
                }
            }
        }
    }

    void draw() {
        window.clear(sf::Color::White);
        
        // Debug positions
        std::cout << "Title position: " << title.getPosition().x << "," << title.getPosition().y << std::endl;
        std::cout << "TitleBox position: " << titleBox.getPosition().x << "," << titleBox.getPosition().y << std::endl;
        std::cout << "Left circle position: " << leftCircle.getPosition().x << "," << leftCircle.getPosition().y << std::endl;
        std::cout << "Right circle position: " << rightCircle.getPosition().x << "," << rightCircle.getPosition().y << std::endl;
        
        window.draw(titleBox);
        window.draw(title);
        window.draw(leftCircle);
        window.draw(rightCircle);
        window.draw(playButton);
        window.draw(playText);
        window.draw(exitButton);
        window.draw(exitText);

        window.display();
    }

    bool isOpen() const {
        return isMenuOpen;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Final StandOff");
    Menu menu(window);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        menu.processEvents();
        menu.draw();
    }

    return 0;
}


