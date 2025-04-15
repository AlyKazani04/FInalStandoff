#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Character.cpp"
#include "enemy.cpp"

class GameState {
private:
    sf::RenderWindow& window;
    Character* player;
    std::vector<Enemy*> enemies;
    sf::Clock clock;
    bool isGameOver;
    
    // Collision detection
    bool checkCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
        return false; //check if the two rectangles intersect //rect1.intersects(rect2)
    }
    
    // Handle player-enemy collisions
    void handleCollisions() {
        if (!player) return;
        
        sf::FloatRect playerBounds = player->getBounds();
        
        for (Enemy* enemy : enemies) {
            if (!enemy) continue;
            
            sf::FloatRect enemyBounds = enemy->getBounds();
            
            if (checkCollision(playerBounds, enemyBounds)) {
                // Handle collision - for now just print a message
                std::cout << "Collision detected between player and enemy!" << std::endl;
                enemy->takeDamage(10.0f);
            }
        }
    }
    // THE FOLLOWING CODE IS COMMENTED BECAUSE I COULDN'T FIGURE OUT HOW TO GET THE KEYPRESSES TO WORK WITHOUT THE EVENT HANDLER!!!!!

    // Handle player input
    // void handleInput() {
    //     if (!player) return;
        
    //     float moveX = 0.0f;
    //     float moveY = 0.0f;
        
    //     if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
    //         if (keyPressed->scancode == sf::Keyboard::Scancode::A) moveX = -2.0f;
    //         if (keyPressed->scancode == sf::Keyboard::Scancode::D) moveX = 2.0f;
    //         if (keyPressed->scancode == sf::Keyboard::Scancode::W) moveY = -2.0f;
    //         if (keyPressed->scancode == sf::Keyboard::Scancode::S) moveY = 2.0f;
        
    //         // Handle attack
    //         if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
    //             player->setAnimation(Character::ATTACK, player->getLastDirection());
    //         }
    //     }
        
    //     if (moveX != 0.0f || moveY != 0.0f) {
    //         player->move(moveX, moveY);
    //     }

    // }
    
    // Update game state
    void update(float deltaTime) {
        if (!player) return;
        
        player->update(deltaTime);
        
        for (Enemy* enemy : enemies) {
            if (enemy) {
                enemy->update(deltaTime);
            }
        }
        
        handleCollisions();
    }
    
    // Render game
    void render() {
        window.clear(sf::Color(50, 50, 50));
        
        if (player) {
            player->draw(window);
        }
        
        for (Enemy* enemy : enemies) {
            if (enemy) {
                enemy->draw(window);
            }
        }
        
        window.display();
    }

public:
    GameState(sf::RenderWindow& gameWindow) : window(gameWindow), isGameOver(false) {
        // Initialize game state
        loadGame();
    }
    
    ~GameState() {
        // Clean up resources
        if (player) {
            delete player;
            player = nullptr;
        }
        
        for (Enemy* enemy : enemies) {
            if (enemy) {
                delete enemy;
            }
        }
        enemies.clear();
    }
    
    // Load game entities
    void loadGame() {
        // Load player
        player = Character::loadCharacter(sf::Vector2f(400, 300));
        
        // Load enemies
        // You can add more enemies with different positions
        enemies.push_back(Enemy::loadEnemy(sf::Vector2f(600, 300)));
        enemies.push_back(Enemy::loadEnemy(sf::Vector2f(200, 300)));
        enemies.push_back(Enemy::loadEnemy(sf::Vector2f(400, 100)));
        enemies.push_back(Enemy::loadEnemy(sf::Vector2f(400, 500)));
    }
    
    // Main game loop
    void run() {
        while (window.isOpen() && !isGameOver) {
            sf::Time deltaTime = clock.restart();
            
            // Handle events
            while (const auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (!player) return;

                float moveX = 0.0f;
                float moveY = 0.0f;
                
                // Handle game over
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        isGameOver = true;
                    }
                    if (keyPressed->scancode == sf::Keyboard::Scancode::A) moveX = -2.0f;
                    if (keyPressed->scancode == sf::Keyboard::Scancode::D) moveX = 2.0f;
                    if (keyPressed->scancode == sf::Keyboard::Scancode::W) moveY = -2.0f;
                    if (keyPressed->scancode == sf::Keyboard::Scancode::S) moveY = 2.0f;
                
                    // Handle attack
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                        player->setAnimation(Character::ATTACK, player->getLastDirection());
                    }

                    if (moveX != 0.0f || moveY != 0.0f) {
                        player->move(moveX, moveY);
                    }
                }
            }
             // Update game state
            update(deltaTime.asSeconds());
            
            // Render game
            render();
        }
    }
    
    // Check if game is over
    bool isGameFinished() const {
        return isGameOver;
    }
};

// Main function to run the game
int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game with Collision");
    window.setFramerateLimit(60);
    
    GameState gameState(window);
    gameState.run();
    
    return 0;
} 