#include <SFML/Graphics.hpp>
#include "ScreenManager.hpp"
#include <iostream>

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Screen Manager Test");
    window.setFramerateLimit(60);

    // Create resources for both screens
    sf::Font font;
    sf::Texture titleBoxTexture;
    sf::Texture buttonTexture;
    
    // Create UI elements for start screen
    sf::Text startTitle(font, "", 64);
    sf::Sprite startTitleBox(titleBoxTexture);
    sf::RectangleShape startPlayButton;
    sf::RectangleShape startExitButton;
    sf::Text startPlayText(font, "", 64);
    sf::Text startExitText(font, "", 64);
    
    // Create UI elements for pause screen
    sf::Text pauseTitle(font, "", 64);
    sf::Sprite pauseTitleBox(titleBoxTexture);
    sf::RectangleShape pauseResumeButton;
    sf::RectangleShape pauseExitButton;
    sf::Text pauseResumeText(font, "", 64);
    sf::Text pauseExitText(font, "", 64);
    
    // Load both screens
    bool startScreenLoaded = ScreenManager::loadStartScreen(
        window, font, titleBoxTexture, buttonTexture,
        startTitle, startTitleBox, startPlayButton, startExitButton,
        startPlayText, startExitText
    );
    
    bool pauseScreenLoaded = ScreenManager::loadPauseScreen(
        window, font, titleBoxTexture, buttonTexture,
        pauseTitle, pauseTitleBox, pauseResumeButton, pauseExitButton,
        pauseResumeText, pauseExitText
    );
    
    if (!startScreenLoaded || !pauseScreenLoaded) {
        std::cout << "Failed to load screens. Exiting..." << std::endl;
        return 1;
    }
    
    std::cout << "Both screens loaded successfully!" << std::endl;
    
    // Game state variables
    enum class GameState {
        START_SCREEN,
        PLAYING,
        PAUSED
    };
    
    GameState currentState = GameState::START_SCREEN;
    bool isRunning = true;
    
    // Main game loop
    while (window.isOpen() && isRunning) {
        // Handle events
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    if (currentState == GameState::PLAYING) {
                        currentState = GameState::PAUSED;
                        std::cout << "Game paused" << std::endl;
                    }
                    else if (currentState == GameState::PAUSED) {
                        currentState = GameState::PLAYING;
                        std::cout << "Game resumed" << std::endl;
                    }
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    if (currentState == GameState::START_SCREEN) {
                        currentState = GameState::PLAYING;
                        std::cout << "Game started" << std::endl;
                    }
                }
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                if (currentState == GameState::START_SCREEN) {
                    if (startPlayButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                        currentState = GameState::PLAYING;
                        std::cout << "Game started (via button)" << std::endl;
                    }
                    else if (startExitButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                        window.close();
                    }
                }
                else if (currentState == GameState::PAUSED) {
                    if (pauseResumeButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                        currentState = GameState::PLAYING;
                        std::cout << "Game resumed (via button)" << std::endl;
                    }
                    else if (pauseExitButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                        window.close();
                    }
                }
            }
        }
        
        // Render based on current state
        if (currentState == GameState::START_SCREEN) {
            ScreenManager::renderStartScreen(
                window, startTitle, startTitleBox,
                startPlayButton, startPlayText,
                startExitButton, startExitText
            );
        }
        else if (currentState == GameState::PAUSED) {
            // For pause screen, we need to render the game state first
            window.clear(sf::Color(50, 50, 50));
            
            // Draw a simple game representation
            sf::RectangleShape gameObject(sf::Vector2f(100, 100));
            gameObject.setFillColor(sf::Color::Green);
            gameObject.setPosition(sf::Vector2f(350, 250));
            window.draw(gameObject);
            
            // Then render the pause screen on top
            ScreenManager::renderPauseScreen(
                window, pauseTitle, pauseTitleBox,
                pauseResumeButton, pauseResumeText,
                pauseExitButton, pauseExitText
            );
        }
        else { // PLAYING state
            window.clear(sf::Color(50, 50, 50));
            
            // Draw a simple game representation
            sf::RectangleShape gameObject(sf::Vector2f(100, 100));
            gameObject.setFillColor(sf::Color::Green);
            gameObject.setPosition(sf::Vector2f(350, 250));
            window.draw(gameObject);
            
            // Draw instructions
            sf::Text instructions(font, "", 20);
            instructions.setFont(font);
            instructions.setString("Press ESC to pause\nPress SPACE to start from start screen");
            instructions.setCharacterSize(20);
            instructions.setFillColor(sf::Color::White);
            instructions.setPosition(sf::Vector2f(10, 10));
            window.draw(instructions);
            
            window.display();
        }
    }
    
    return 0;
} 