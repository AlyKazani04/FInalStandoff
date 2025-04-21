#include "MenuFunctions.hpp"

bool loadMenuResources(sf::Font& font, sf::Texture& titleBoxTexture, sf::Texture& buttonTexture,
                      const std::string& fontPath, const std::string& titleBoxPath, 
                      const std::string& buttonPath) {
    // Load font
    if (!font.openFromFile(fontPath)) {
        std::cout << "Error loading font" << std::endl;
        return false;
    }

    // Load title box texture
    if (!titleBoxTexture.loadFromFile(titleBoxPath)) {
        std::cout << "Error loading title box texture" << std::endl;
        return false;
    }

    // Load button texture
    if (!buttonTexture.loadFromFile(buttonPath)) {
        std::cout << "Error loading button texture" << std::endl;
        return false;
    }

    return true;
}

void setupTitleAndBox(sf::Text& title, sf::Sprite& titleBox, const sf::Font& font, 
                     const sf::Texture& titleBoxTexture, const sf::Vector2u& windowSize) {
    // Setup title text first to get its dimensions
    title.setString("Final StandOff");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::White);
    
    // Get the title text dimensions
    float titleWidth = title.getLocalBounds().size.x;
    float titleHeight = title.getLocalBounds().size.y;
    
    // Add padding around the text
    float padding = 40.0f;  // Padding on each side
    
    // Calculate required scale for the title box to fit the text plus padding
    float scaleX = (titleWidth + padding * 2) / titleBoxTexture.getSize().x;
    float scaleY = (titleHeight + padding * 2) / titleBoxTexture.getSize().y;
    titleBox.setScale(sf::Vector2f(scaleX, scaleY));
    
    // Setup title box
    titleBox.setTexture(titleBoxTexture);
    
    // Center vertically and horizontally
    float verticalCenter = windowSize.y / 2.0f - 100.0f;  // Moved up slightly
    
    // Calculate the actual dimensions of the scaled title box
    float titleBoxWidth = titleBoxTexture.getSize().x * titleBox.getScale().x;
    float titleBoxHeight = titleBoxTexture.getSize().y * titleBox.getScale().y;
    
    // Position the title box
    titleBox.setPosition(sf::Vector2f(
        windowSize.x / 2.0f - titleBoxWidth / 2.0f,
        verticalCenter - titleBoxHeight / 2.0f
    ));
    
    // Position the text exactly in the center of the title box
    title.setPosition(sf::Vector2f(
        titleBox.getPosition().x + (titleBoxWidth - titleWidth) / 2.0f,
        titleBox.getPosition().y + (titleBoxHeight - titleHeight) / 2.0f - 10.0f  // Slight upward adjustment
    ));
}

void setupButtons(sf::RectangleShape& playButton, sf::RectangleShape& exitButton,
                 sf::Text& playText, sf::Text& exitText, const sf::Font& font,
                 const sf::Vector2u& windowSize, const sf::Texture& buttonTexture) {
    // Setup play button
    playButton.setSize(sf::Vector2f(200.0f, 50.0f));
    playButton.setPosition(sf::Vector2f(
        windowSize.x / 2.0f - 100.0f,
        300.0f
    ));
    playButton.setTexture(&buttonTexture);
    playButton.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(buttonTexture.getSize().x, buttonTexture.getSize().y)));

    // Setup exit button
    exitButton.setSize(sf::Vector2f(200.0f, 50.0f));
    exitButton.setPosition(sf::Vector2f(
        windowSize.x / 2.0f - 100.0f,
        400.0f
    ));
    exitButton.setTexture(&buttonTexture);
    exitButton.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(buttonTexture.getSize().x, buttonTexture.getSize().y)));

    // Setup text for buttons
    playText.setString("Play");
    playText.setCharacterSize(30);
    playText.setFillColor(sf::Color::White);
    playText.setPosition(sf::Vector2f(
        playButton.getPosition().x + (200.0f - playText.getLocalBounds().size.x) / 2,
        playButton.getPosition().y + (50.0f - playText.getCharacterSize()) / 2
    ));

    exitText.setString("Exit");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(sf::Vector2f(
        exitButton.getPosition().x + (200.0f - exitText.getLocalBounds().size.x) / 2,
        exitButton.getPosition().y + (50.0f - exitText.getCharacterSize()) / 2
    ));
}

bool processMenuEvents(sf::RenderWindow& window, sf::RectangleShape& playButton, 
                      sf::RectangleShape& exitButton, bool& isMenuOpen) {
    while (window.isOpen()) {
        while(const std::optional<sf::Event> event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
                isMenuOpen = false;
                return false;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                    isMenuOpen = false;
                    return false;
                }
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()){
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (playButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    // Handle play button click
                    std::cout << "Play clicked!" << std::endl;
                    isMenuOpen = false;
                    return true;
                }
                else if (exitButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    window.close();
                    isMenuOpen = false;
                    return false;
                }
            }
            // Add hover effects
            else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                updateButtonHoverEffects(playButton, exitButton, mousePos);
            }
        }
        return true;
    }
}

void drawMenu(sf::RenderWindow& window, const sf::Text& title, const sf::Sprite& titleBox,
             const sf::RectangleShape& playButton, const sf::Text& playText,
             const sf::RectangleShape& exitButton, const sf::Text& exitText) {
    window.clear(sf::Color::Black);
    
    window.draw(titleBox);
    window.draw(title);
    window.draw(playButton);
    window.draw(playText);
    window.draw(exitButton);
    window.draw(exitText);

    window.display();
}

void updateButtonHoverEffects(sf::RectangleShape& playButton, sf::RectangleShape& exitButton,
                            const sf::Vector2i& mousePos) {
    if (playButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
        playButton.setFillColor(sf::Color(200, 200, 200));  // Slightly darker when hovered
    } else {
        playButton.setFillColor(sf::Color::White);  // Normal color
    }
    if (exitButton.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
        exitButton.setFillColor(sf::Color(200, 200, 200));
    } else {
        exitButton.setFillColor(sf::Color::White);  // Normal color
    }
} 
                      