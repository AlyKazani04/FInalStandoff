#include <SFML/Graphics.hpp>
#include "PauseMenuFunctions.hpp"
#include "PauseMenu.hpp"

void pauseMenu() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Final StandOff");
    window.setFramerateLimit(60);

    // Create menu resources
    sf::Font font;
    sf::Texture titleBoxTexture;
    sf::Texture buttonTexture;
    
    // Load resources
    bool resourcesLoaded = loadMenuResources(
        font, 
        titleBoxTexture, 
        buttonTexture,
        "/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/fonts/Blacknorthdemo-mLE25.otf",  // Font path
        "/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/UI/MenusBox_34x34.png",  // Title box path
        "/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/UI/Button_52x14.png"    // Button path
    );
    
    if (!resourcesLoaded) {
        std::cout << "Failed to load resources. Exiting..." << std::endl;
        return;
    }
    
    // Create menu elements
    sf::Text title(font, "", 64);
    sf::Sprite titleBox(titleBoxTexture);
    
    sf::RectangleShape playButton;
    sf::RectangleShape exitButton;
    sf::Text playText(font, "", 30);
    sf::Text exitText(font, "", 30);
    
    // Setup menu elements
    setupTitleAndBox(title, titleBox, font, titleBoxTexture, window.getSize());
    setupButtons(playButton, exitButton, playText, exitText, font, window.getSize(), buttonTexture);
    
    // Menu loop
    bool isMenuOpen = true;
    bool shouldPauseGame = false;
    
    while (window.isOpen() && isMenuOpen) {
        // Process events
        shouldPauseGame = processMenuEvents(window, playButton, exitButton, isMenuOpen);
        
        // Draw Pause menu
        drawMenu(window, title, titleBox, playButton, playText, exitButton, exitText);
    }
    
    // If the game should Pause
    if (shouldPauseGame) {
        std::cout << "Game Paused..." << std::endl;
        // Add your game initialization code here
    }
} 