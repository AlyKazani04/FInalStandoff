#include <iostream>
#include "ScreenManager.hpp"
#include "constants.cpp"

ScreenManager::ScreenManager()
    : m_font()
    , m_titleBoxTexture()
    , m_buttonTexture()
    , m_startTitle(m_font, "" , 64)
    , m_startTitleBox(m_titleBoxTexture)
    , m_startPlayButton()
    , m_startExitButton()
    , m_startPlayText(m_font, "" , 32)
    , m_startExitText(m_font, "" , 32)
    , m_pauseTitle(m_font, "" , 64)
    , m_pauseTitleBox(m_titleBoxTexture)
    , m_pauseResumeButton()
    , m_pauseExitButton()
    , m_pauseResumeText(m_font, "" , 32)
    , m_pauseExitText(m_font, "" , 32)
{
    // Constructor body is empty - all initialization is done in the initialization list
}

ScreenManager::~ScreenManager() {
    // Default Destructor
}

bool ScreenManager::initialize(sf::RenderWindow& window) {
    // Load resources
    if (!loadResources()) {
        std::cout << "Failed to load resources" << std::endl;
        return false;
    }

    // Setup screens
    if (!setupStartScreen(window) || !setupPauseScreen(window)) {
        std::cout << "Failed to setup screens" << std::endl;
        return false;
    }

    return true;
}

bool ScreenManager::loadResources() {
    // Load font
    if (!m_font.openFromFile(FONT_PATH)) {
        std::cout << "Failed to load font from " << FONT_PATH << std::endl;
        return false;
    }

    // Load textures
    if (!m_titleBoxTexture.loadFromFile(TITLE_BOX_PATH)) {
        std::cout << "Failed to load title box texture from " << TITLE_BOX_PATH << std::endl;
        return false;
    }

    if (!m_buttonTexture.loadFromFile(BUTTON_PATH)) {
        std::cout << "Failed to load button texture from " << BUTTON_PATH << std::endl;
        return false;
    }

    return true;
}

bool ScreenManager::setupStartScreen(sf::RenderWindow& window) {
    // Setup start title
    m_startTitle.setFont(m_font);
    m_startTitle.setString("FINAL STANDOFF");
    m_startTitle.setCharacterSize(64);
    m_startTitle.setFillColor(sf::Color::White);
    
    // Center the title
    sf::FloatRect titleBounds = m_startTitle.getLocalBounds();
    m_startTitle.setOrigin(sf::Vector2f(titleBounds.size.x / 2, titleBounds.size.y / 2));
    m_startTitle.setPosition(sf::Vector2f(window.getView().getSize().x / 2, window.getView().getSize().y / 3));

    // Setup title box
    m_startTitleBox.setTexture(m_titleBoxTexture);
    m_startTitleBox.setOrigin(sf::Vector2f(m_titleBoxTexture.getSize().x / 2, m_titleBoxTexture.getSize().y / 2));
    m_startTitleBox.setPosition(m_startTitle.getPosition());
    m_startTitleBox.setScale(sf::Vector2f(1.5f, 1.5f));

    // Setup play button
    m_startPlayButton.setSize(sf::Vector2f(200, 60));
    m_startPlayButton.setTexture(&m_buttonTexture);
    m_startPlayButton.setOrigin(sf::Vector2f(100, 30));
    m_startPlayButton.setPosition(sf::Vector2f(window.getView().getSize().x / 2, window.getView().getSize().y / 2));

    // Setup play text
    m_startPlayText.setFont(m_font);
    m_startPlayText.setString("Play");
    m_startPlayText.setCharacterSize(32);
    m_startPlayText.setFillColor(sf::Color::White);
    
    // Center the play text
    sf::FloatRect playTextBounds = m_startPlayText.getLocalBounds();
    m_startPlayText.setOrigin(sf::Vector2f(playTextBounds.size.x / 2, playTextBounds.size.y / 2));
    m_startPlayText.setPosition(m_startPlayButton.getPosition());

    // Setup exit button
    m_startExitButton.setSize(sf::Vector2f(200, 60));
    m_startExitButton.setTexture(&m_buttonTexture);
    m_startExitButton.setOrigin(sf::Vector2f(100, 30));
    m_startExitButton.setPosition(sf::Vector2f(window.getView().getSize().x / 2, window.getView().getSize().y / 2 + 80));

    // Setup exit text
    m_startExitText.setFont(m_font);
    m_startExitText.setString("Exit");
    m_startExitText.setCharacterSize(32);
    m_startExitText.setFillColor(sf::Color::White);
    
    // Center the exit text
    sf::FloatRect exitTextBounds = m_startExitText.getLocalBounds();
    m_startExitText.setOrigin(sf::Vector2f(exitTextBounds.size.x / 2, exitTextBounds.size.y / 2));
    m_startExitText.setPosition(m_startExitButton.getPosition());

    return true;
}

bool ScreenManager::setupPauseScreen(sf::RenderWindow& window) {
    // Setup pause title
    m_pauseTitle.setFont(m_font);
    m_pauseTitle.setString("Paused");
    m_pauseTitle.setCharacterSize(64);
    m_pauseTitle.setFillColor(sf::Color::White);
    
    // Center the title
    sf::FloatRect titleBounds = m_pauseTitle.getLocalBounds();
    m_pauseTitle.setOrigin(sf::Vector2f(titleBounds.size.x / 2, titleBounds.size.y / 2));
    m_pauseTitle.setPosition(sf::Vector2f(window.getView().getSize().x / 2, window.getView().getSize().y / 3));

    // Setup title box
    m_pauseTitleBox.setTexture(m_titleBoxTexture);
    m_pauseTitleBox.setOrigin(sf::Vector2f(m_titleBoxTexture.getSize().x / 2, m_titleBoxTexture.getSize().y / 2));
    m_pauseTitleBox.setPosition(m_pauseTitle.getPosition());
    m_pauseTitleBox.setScale(sf::Vector2f(1.5f, 1.5f));

    // Setup resume button
    m_pauseResumeButton.setSize(sf::Vector2f(200, 60));
    m_pauseResumeButton.setTexture(&m_buttonTexture);
    m_pauseResumeButton.setOrigin(sf::Vector2f(100, 30));
    m_pauseResumeButton.setPosition(sf::Vector2f(window.getView().getSize().x / 2, window.getView().getSize().y / 2));

    // Setup resume text
    m_pauseResumeText.setFont(m_font);
    m_pauseResumeText.setString("Resume");
    m_pauseResumeText.setCharacterSize(32);
    m_pauseResumeText.setFillColor(sf::Color::White);
    
    // Center the resume text
    sf::FloatRect resumeTextBounds = m_pauseResumeText.getLocalBounds();
    m_pauseResumeText.setOrigin(sf::Vector2f(resumeTextBounds.size.x / 2, resumeTextBounds.size.y / 2));
    m_pauseResumeText.setPosition(m_pauseResumeButton.getPosition());

    // Setup exit button
    m_pauseExitButton.setSize(sf::Vector2f(200, 60));
    m_pauseExitButton.setTexture(&m_buttonTexture);
    m_pauseExitButton.setOrigin(sf::Vector2f(100, 30));
    m_pauseExitButton.setPosition(sf::Vector2f(window.getView().getSize().x / 2, window.getView().getSize().y / 2 + 80));

    // Setup exit text
    m_pauseExitText.setFont(m_font);
    m_pauseExitText.setString("Exit");
    m_pauseExitText.setCharacterSize(32);
    m_pauseExitText.setFillColor(sf::Color::White);
    
    // Center the exit text
    sf::FloatRect exitTextBounds = m_pauseExitText.getLocalBounds();
    m_pauseExitText.setOrigin(sf::Vector2f(exitTextBounds.size.x / 2, exitTextBounds.size.y / 2));
    m_pauseExitText.setPosition(m_pauseExitButton.getPosition());

    return true;
}

bool ScreenManager::handleStartScreenInput(sf::RenderWindow& window, bool& startGame, bool& exitGame) {
    startGame = false;
    exitGame = false;
    
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    
    if (m_startPlayButton.getGlobalBounds().contains(sf::Vector2f(worldPos.x, worldPos.y)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        startGame = true;
    }
    if (m_startExitButton.getGlobalBounds().contains(sf::Vector2f(worldPos.x, worldPos.y)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        exitGame = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
        startGame = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Backspace)) {
        exitGame = true;
    }

    return true;
}

bool ScreenManager::handlePauseScreenInput(sf::RenderWindow& window, bool& resumeGame, bool& exitGame) {
    resumeGame = false;
    exitGame = false;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    
    if (m_pauseResumeButton.getGlobalBounds().contains(sf::Vector2f(worldPos.x, worldPos.y)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        resumeGame = true;
    }
    if (m_pauseExitButton.getGlobalBounds().contains(sf::Vector2f(worldPos.x, worldPos.y)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        exitGame = true;
    }
    return true;
}

void ScreenManager::renderStartScreen(sf::RenderWindow& window) {
    // Draw title box and title
    window.draw(m_startTitleBox);
    window.draw(m_startTitle);
    
    // Draw buttons and text
    window.draw(m_startPlayButton);
    window.draw(m_startPlayText);
    window.draw(m_startExitButton);
    window.draw(m_startExitText);
    
    window.display();
}

void ScreenManager::renderPauseScreen(sf::RenderWindow& window) {
    // Draw title box and title
    window.draw(m_pauseTitleBox);
    window.draw(m_pauseTitle);
    
    // Draw buttons and text
    window.draw(m_pauseResumeButton);
    window.draw(m_pauseResumeText);
    window.draw(m_pauseExitButton);
    window.draw(m_pauseExitText);
    
    window.display();
} 
