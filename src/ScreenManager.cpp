#include "ScreenManager.hpp"
#include <iostream>

const std::string ScreenManager::FONT_PATH = "/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/fonts/Blacknorthdemo-mLE25.otf";
const std::string ScreenManager::TITLE_BOX_PATH = "/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/UI/MenusBox_34x34.png";
const std::string ScreenManager::BUTTON_PATH = "/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/UI/Button_52x14.png";

bool ScreenManager::loadStartScreen(sf::RenderWindow& window,
                                  sf::Font& font,
                                  sf::Texture& titleBoxTexture,
                                  sf::Texture& buttonTexture,
                                  sf::Text& title,
                                  sf::Sprite& titleBox,
                                  sf::RectangleShape& playButton,
                                  sf::RectangleShape& exitButton,
                                  sf::Text& playText,
                                  sf::Text& exitText) {
    // Load resources
    if (!font.openFromFile(FONT_PATH)) {
        std::cout << "Error loading font" << std::endl;
        return false;
    }

    if (!titleBoxTexture.loadFromFile(TITLE_BOX_PATH)) {
        std::cout << "Error loading title box texture" << std::endl;
        return false;
    }

    if (!buttonTexture.loadFromFile(BUTTON_PATH)) {
        std::cout << "Error loading button texture" << std::endl;
        return false;
    }

    // Setup title
    title.setFont(font);
    title.setString("Final StandOff");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::White);
    
    // Setup title box
    titleBox.setTexture(titleBoxTexture);
    float titleWidth = title.getLocalBounds().size.x;
    float titleHeight = title.getLocalBounds().size.y;
    float padding = 40.0f;
    float scaleX = (titleWidth + padding * 2) / titleBoxTexture.getSize().x;
    float scaleY = (titleHeight + padding * 2) / titleBoxTexture.getSize().y;
    titleBox.setScale(sf::Vector2f(scaleX, scaleY));

    // Position title and title box
    float verticalCenter = window.getSize().y / 2.0f - 100.0f;
    float titleBoxWidth = titleBoxTexture.getSize().x * scaleX;
    float titleBoxHeight = titleBoxTexture.getSize().y * scaleY;
    
    titleBox.setPosition(
        sf::Vector2f(window.getSize().x / 2.0f - titleBoxWidth / 2.0f,
        verticalCenter - titleBoxHeight / 2.0f)
    );
    
    title.setPosition(
        sf::Vector2f(titleBox.getPosition().x + (titleBoxWidth - titleWidth) / 2.0f,
        titleBox.getPosition().y + (titleBoxHeight - titleHeight) / 2.0f - 10.0f)
    );

    // Setup buttons
    playButton.setSize(sf::Vector2f(200.0f, 50.0f));
    playButton.setPosition(
        sf::Vector2f(window.getSize().x / 2.0f - 100.0f,
        300.0f)
    );
    playButton.setTexture(&buttonTexture);

    exitButton.setSize(sf::Vector2f(200.0f, 50.0f));
    exitButton.setPosition(
        sf::Vector2f(window.getSize().x / 2.0f - 100.0f,
        400.0f)
    );
    exitButton.setTexture(&buttonTexture);

    // Setup button text
    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(30);
    playText.setFillColor(sf::Color::White);
    playText.setPosition(
        sf::Vector2f(playButton.getPosition().x + (200.0f - playText.getLocalBounds().size.x) / 2,
        playButton.getPosition().y + (50.0f - playText.getCharacterSize()) / 2)
    );

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(
        sf::Vector2f(exitButton.getPosition().x + (200.0f - exitText.getLocalBounds().size.x) / 2,
        exitButton.getPosition().y + (50.0f - exitText.getCharacterSize()) / 2)
    );

    return true;
}

bool ScreenManager::loadPauseScreen(sf::RenderWindow& window,
                                  sf::Font& font,
                                  sf::Texture& titleBoxTexture,
                                  sf::Texture& buttonTexture,
                                  sf::Text& title,
                                  sf::Sprite& titleBox,
                                  sf::RectangleShape& resumeButton,
                                  sf::RectangleShape& exitButton,
                                  sf::Text& resumeText,
                                  sf::Text& exitText) {
    // Load resources (if not already loaded)
    if (!font.openFromFile(FONT_PATH)) {
        std::cout << "Error loading font" << std::endl;
        return false;
    }

    if (!titleBoxTexture.loadFromFile(TITLE_BOX_PATH)) {
        std::cout << "Error loading title box texture" << std::endl;
        return false;
    }

    if (!buttonTexture.loadFromFile(BUTTON_PATH)) {
        std::cout << "Error loading button texture" << std::endl;
        return false;
    }

    // Setup title
    title.setFont(font);
    title.setString("Paused");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::White);
    
    // Setup title box
    titleBox.setTexture(titleBoxTexture);
    float titleWidth = title.getLocalBounds().size.x;
    float titleHeight = title.getLocalBounds().size.y;
    float padding = 40.0f;
    float scaleX = (titleWidth + padding * 2) / titleBoxTexture.getSize().x;
    float scaleY = (titleHeight + padding * 2) / titleBoxTexture.getSize().y;
    titleBox.setScale(sf::Vector2f(scaleX, scaleY));

    // Position title and title box
    float verticalCenter = window.getSize().y / 2.0f - 100.0f;
    float titleBoxWidth = titleBoxTexture.getSize().x * scaleX;
    float titleBoxHeight = titleBoxTexture.getSize().y * scaleY;
    
    titleBox.setPosition(
        sf::Vector2f(window.getSize().x / 2.0f - titleBoxWidth / 2.0f,
        verticalCenter - titleBoxHeight / 2.0f)
    );
    
    title.setPosition(
        sf::Vector2f(titleBox.getPosition().x + (titleBoxWidth - titleWidth) / 2.0f,
        titleBox.getPosition().y + (titleBoxHeight - titleHeight) / 2.0f - 10.0f)
    );

    // Setup buttons
    resumeButton.setSize(sf::Vector2f(200.0f, 50.0f));
    resumeButton.setPosition(
        sf::Vector2f(window.getSize().x / 2.0f - 100.0f,
        300.0f)
    );
    resumeButton.setTexture(&buttonTexture);

    exitButton.setSize(sf::Vector2f(200.0f, 50.0f));
    exitButton.setPosition(
        sf::Vector2f(window.getSize().x / 2.0f - 100.0f,
        400.0f)
    );
    exitButton.setTexture(&buttonTexture);

    // Setup button text
    resumeText.setFont(font);
    resumeText.setString("Resume");
    resumeText.setCharacterSize(30);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(
        sf::Vector2f(resumeButton.getPosition().x + (200.0f - resumeText.getLocalBounds().size.x) / 2,
        resumeButton.getPosition().y + (50.0f - resumeText.getCharacterSize()) / 2)
    );

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(
        sf::Vector2f(exitButton.getPosition().x + (200.0f - exitText.getLocalBounds().size.x) / 2,
        exitButton.getPosition().y + (50.0f - exitText.getCharacterSize()) / 2)
    );

    return true;
}

void ScreenManager::renderStartScreen(sf::RenderWindow& window,
                                    const sf::Text& title,
                                    const sf::Sprite& titleBox,
                                    const sf::RectangleShape& playButton,
                                    const sf::Text& playText,
                                    const sf::RectangleShape& exitButton,
                                    const sf::Text& exitText) {
    window.clear(sf::Color::Black);
    
    window.draw(titleBox);
    window.draw(title);
    window.draw(playButton);
    window.draw(playText);
    window.draw(exitButton);
    window.draw(exitText);
    
    window.display();
}

void ScreenManager::renderPauseScreen(sf::RenderWindow& window,
                                    const sf::Text& title,
                                    const sf::Sprite& titleBox,
                                    const sf::RectangleShape& resumeButton,
                                    const sf::Text& resumeText,
                                    const sf::RectangleShape& exitButton,
                                    const sf::Text& exitText) {
    window.clear(sf::Color(0, 0, 0, 200));  // Semi-transparent black background
    
    window.draw(titleBox);
    window.draw(title);
    window.draw(resumeButton);
    window.draw(resumeText);
    window.draw(exitButton);
    window.draw(exitText);
    
    window.display();
} 