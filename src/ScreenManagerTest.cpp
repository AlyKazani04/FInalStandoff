#include <SFML/Graphics.hpp>
#include "ScreenManager.hpp"
#include <iostream>

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Screen Manager Test");
    window.setFramerateLimit(60);

    // Create and initialize the screen manager
    ScreenManager screenManager;
    if (!screenManager.initialize(window)) {
        std::cout << "Failed to initialize screen manager. Exiting..." << std::endl;
        return 1;
    }
    
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
        // Handle events based on current state
        if (currentState == GameState::START_SCREEN) {
            bool startGame = false;
            bool exitGame = false;
            
            if (!screenManager.handleStartScreenInput(window, startGame, exitGame)) {
                isRunning = false;
                break;
            }
            
            if (startGame) {
                currentState = GameState::PLAYING;
                std::cout << "Game started" << std::endl;
            }
            else if (exitGame) {
                isRunning = false;
                break;
            }
            
            // Render start screen
            screenManager.renderStartScreen(window);
        }
        else if (currentState == GameState::PAUSED) {
            bool resumeGame = false;
            bool exitGame = false;
            
            if (!screenManager.handlePauseScreenInput(window, resumeGame, exitGame)) {
                isRunning = false;
                break;
            }
            
            if (resumeGame) {
                currentState = GameState::PLAYING;
                std::cout << "Game resumed" << std::endl;
            }
            else if (exitGame) {
                isRunning = false;
                break;
            }
            
            // Render pause screen
            screenManager.renderPauseScreen(window);
        }
        else { // PLAYING state
            // Handle events
            while (const auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                    isRunning = false;
                    break;
                }
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        currentState = GameState::PAUSED;
                        std::cout << "Game paused" << std::endl;
                        break;
                    }
                }
            }
            
            if (!isRunning) break;
            
            // Render game
            window.clear(sf::Color(50, 50, 50));
            
            // Draw a simple game representation
            sf::RectangleShape gameObject(sf::Vector2f(100, 100));
            gameObject.setFillColor(sf::Color::Green);
            gameObject.setPosition(sf::Vector2f(350, 250));
            window.draw(gameObject);
            
            // Draw instructions
            // sf::Text instructions;
            // instructions.setFont(screenManager.getFont());
            // instructions.setString("Press ESC to pause");
            // instructions.setCharacterSize(20);
            // instructions.setFillColor(sf::Color::White);
            // instructions.setPosition(sf::Vector2f(10, 10));
            // window.draw(instructions);
            
            window.display();
        }
    }
    
    return 0;
} 
