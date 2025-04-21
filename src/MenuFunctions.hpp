#ifndef MENU_FUNCTIONS_HPP
#define MENU_FUNCTIONS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <optional>

// Function to load resources (fonts, textures)
bool loadMenuResources(sf::Font& font, sf::Texture& titleBoxTexture, sf::Texture& buttonTexture,
                      const std::string& fontPath, const std::string& titleBoxPath, 
                      const std::string& buttonPath);

// Function to setup the title and title box
void setupTitleAndBox(sf::Text& title, sf::Sprite& titleBox, const sf::Font& font, 
                     const sf::Texture& titleBoxTexture, const sf::Vector2u& windowSize);

// Function to setup buttons
void setupButtons(sf::RectangleShape& playButton, sf::RectangleShape& exitButton,
                 sf::Text& playText, sf::Text& exitText, const sf::Font& font,
                 const sf::Vector2u& windowSize, const sf::Texture& buttonTexture);

// Function to handle menu events
bool processMenuEvents(sf::RenderWindow& window, sf::RectangleShape& playButton, 
                      sf::RectangleShape& exitButton, bool& isMenuOpen);

// Function to draw the menu
void drawMenu(sf::RenderWindow& window, const sf::Text& title, const sf::Sprite& titleBox,
             const sf::RectangleShape& playButton, const sf::Text& playText,
             const sf::RectangleShape& exitButton, const sf::Text& exitText);

// Function to update button hover effects
void updateButtonHoverEffects(sf::RectangleShape& playButton, sf::RectangleShape& exitButton,
                            const sf::Vector2i& mousePos);

#endif // MENU_FUNCTIONS_HPP 