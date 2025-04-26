#ifndef SCREEN_MANAGER_HPP
#define SCREEN_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <string>

class ScreenManager {
public:
    static bool loadStartScreen(sf::RenderWindow& window, 
                              sf::Font& font,
                              sf::Texture& titleBoxTexture,
                              sf::Texture& buttonTexture,
                              sf::Text& title,
                              sf::Sprite& titleBox,
                              sf::RectangleShape& playButton,
                              sf::RectangleShape& exitButton,
                              sf::Text& playText,
                              sf::Text& exitText);

    static bool loadPauseScreen(sf::RenderWindow& window,
                              sf::Font& font,
                              sf::Texture& titleBoxTexture,
                              sf::Texture& buttonTexture,
                              sf::Text& title,
                              sf::Sprite& titleBox,
                              sf::RectangleShape& resumeButton,
                              sf::RectangleShape& exitButton,
                              sf::Text& resumeText,
                              sf::Text& exitText);

    static void renderStartScreen(sf::RenderWindow& window,
                                const sf::Text& title,
                                const sf::Sprite& titleBox,
                                const sf::RectangleShape& playButton,
                                const sf::Text& playText,
                                const sf::RectangleShape& exitButton,
                                const sf::Text& exitText);

    static void renderPauseScreen(sf::RenderWindow& window,
                                const sf::Text& title,
                                const sf::Sprite& titleBox,
                                const sf::RectangleShape& resumeButton,
                                const sf::Text& resumeText,
                                const sf::RectangleShape& exitButton,
                                const sf::Text& exitText);

private:
    static const std::string FONT_PATH;
    static const std::string TITLE_BOX_PATH;
    static const std::string BUTTON_PATH;
};

#endif // SCREEN_MANAGER_HPP 