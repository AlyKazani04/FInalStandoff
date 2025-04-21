#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class MenuEnhanced {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text title;
    sf::RectangleShape playButton;
    sf::RectangleShape exitButton;
    sf::Text playText;
    sf::Text exitText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Music backgroundMusic;
    bool isMenuOpen;
    bool isHoveringPlay;
    bool isHoveringExit;

public:
    MenuEnhanced(sf::RenderWindow& window) : window(window), isMenuOpen(true), isHoveringPlay(false), isHoveringExit(false) {
        // Load font
        if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cout << "Error loading font" << std::endl;
            return;
        }

        // Load background image
        if (!backgroundTexture.loadFromFile("background.jpg")) {  // You'll need to add your own background image
            std::cout << "Error loading background image" << std::endl;
            return;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );

        // Load background music
        if (!backgroundMusic.openFromFile("background_music.ogg")) {  // You'll need to add your own music file
            std::cout << "Error loading background music" << std::endl;
            return;
        }
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();

        // Setup title with shadow effect
        title.setFont(font);
        title.setString("My Game");
        title.setCharacterSize(64);
        title.setFillColor(sf::Color::White);
        title.setPosition(
            window.getSize().x / 2 - title.getGlobalBounds().width / 2,
            100
        );

        // Setup play button with gradient effect
        playButton.setSize(sf::Vector2f(200, 50));
        playButton.setPosition(
            window.getSize().x / 2 - 100,
            300
        );
        playButton.setFillColor(sf::Color(0, 180, 0));  // Green
        playButton.setOutlineThickness(2);
        playButton.setOutlineColor(sf::Color::White);

        playText.setFont(font);
        playText.setString("Play");
        playText.setCharacterSize(30);
        playText.setFillColor(sf::Color::White);
        playText.setPosition(
            playButton.getPosition().x + 80,
            playButton.getPosition().y + 10
        );

        // Setup exit button with gradient effect
        exitButton.setSize(sf::Vector2f(200, 50));
        exitButton.setPosition(
            window.getSize().x / 2 - 100,
            400
        );
        exitButton.setFillColor(sf::Color(180, 0, 0));  // Red
        exitButton.setOutlineThickness(2);
        exitButton.setOutlineColor(sf::Color::White);

        exitText.setFont(font);
        exitText.setString("Exit");
        exitText.setCharacterSize(30);
        exitText.setFillColor(sf::Color::White);
        exitText.setPosition(
            exitButton.getPosition().x + 80,
            exitButton.getPosition().y + 10
        );
    }

    void processEvents() {
        while(const auto event = window.pollEvent()) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                if (playButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isMenuOpen = false;
                }
                else if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Hover effect for play button
                if (playButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    playButton.setFillColor(sf::Color(0, 200, 0));  // Brighter green
                    isHoveringPlay = true;
                } else {
                    playButton.setFillColor(sf::Color(0, 180, 0));  // Normal green
                    isHoveringPlay = false;
                }
                
                // Hover effect for exit button
                if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    exitButton.setFillColor(sf::Color(200, 0, 0));  // Brighter red
                    isHoveringExit = true;
                } else {
                    exitButton.setFillColor(sf::Color(180, 0, 0));  // Normal red
                    isHoveringExit = false;
                }
            }
        }
    }

    void draw() {
        window.clear(sf::Color::Black);
        
        // Draw background
        window.draw(backgroundSprite);
        
        // Draw title with shadow
        sf::Text titleShadow = title;
        titleShadow.setPosition(title.getPosition().x + 2, title.getPosition().y + 2);
        titleShadow.setFillColor(sf::Color(0, 0, 0, 128));  // Semi-transparent black
        window.draw(titleShadow);
        window.draw(title);
        
        // Draw buttons with hover effects
        window.draw(playButton);
        window.draw(playText);
        window.draw(exitButton);
        window.draw(exitText);
        
        window.display();
    }

    bool isOpen() const {
        return isMenuOpen;
    }

    ~MenuEnhanced() {
        backgroundMusic.stop();
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My Game");
    MenuEnhanced menu(window);

    while (window.isOpen()) {
        menu.processEvents();
        menu.draw();
    }

    return 0;
} 