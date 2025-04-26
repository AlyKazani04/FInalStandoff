#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "constants.cpp"
#include "levels.cpp"

sf::Texture& getGlobalTexture(){
    static sf::Texture texture;
    if(!texture.loadFromFile("/Users/syed/Desktop/OOP PROJECT/OOP_PROJECT/Assets/Characters/POGO.png")){
        std::cerr << "Error loading texture" << std::endl;

    }
    texture.setSmooth(false);
    return texture;
}

class Enemy {
    private:
        sf::Sprite sprite;
        sf::Texture idleTexture;
        sf::Texture attackTexture;
        sf::Texture moveTexture;
        sf::Texture deathTexture;
        sf::Texture damageTexture;

        
        std::vector<sf::IntRect> idleFrames;
        std::vector<sf::IntRect> attackFrames;
        std::vector<sf::IntRect> moveFrames;
        std::vector<sf::IntRect> deathFrames;
        std::vector<sf::IntRect> damageFrames;
        
        // Health bar shapes
        sf::RectangleShape healthBarBackground;
        sf::RectangleShape healthBarFill;
        float maxHealth;
        float currentHealth;

        //hitbox
        sf::RectangleShape hitbox;

        sf::Time frameDuration = sf::seconds(0.1f); // in seconds, 0.1secs; 10fps
        int currentFrame;
        int currentAnimation;
        bool isFacingRight;
        bool isDead;
        bool isAttacking;
        bool isTakingDamage;
        int LevelNumber;
        sf::Clock clock;
        sf::Clock deathTimer;
        bool deathAnimationFinished = false;

    public:
        enum AnimationState {
            IDLE = 0,
            MOVE = 1,
            ATTACK = 2,
            DEATH = 3,
            TAKE_DAMAGE = 4
        };

        Enemy() : currentFrame(0), currentAnimation(IDLE), 
                isFacingRight(true), isDead(false), isAttacking(false), isTakingDamage(false),
                sprite(getGlobalTexture()),
                maxHealth(100.f), currentHealth(100.f) {}

        void load(int level){


            // initialize enemy variables
            LevelNumber = level;
            currentHealth = MAX_HEALTH;
            isDead = false;
            isAttacking = false;
            currentFrame = 0;
            currentAnimation = IDLE;
            isFacingRight = true;

            // Setup health bar
            float healthBarWidth = 50.f;
            float healthBarHeight = 5.f;
            
            healthBarBackground.setSize(sf::Vector2f(healthBarWidth, healthBarHeight));
            healthBarBackground.setFillColor(sf::Color(100, 100, 100)); // Dark gray background
            
            healthBarFill.setSize(sf::Vector2f(healthBarWidth, healthBarHeight));
            healthBarFill.setFillColor(sf::Color::Green);

            // Load all textures
            if (!idleTexture.loadFromFile("../resources/EnemyAnim/enemies-skeleton1_idle.png")) {
                std::cout << "Error loading enemy idle texture" << std::endl;
                return;
            }
            if (!attackTexture.loadFromFile("../resources/EnemyAnim/enemies-skeleton1_attack.png")) {
                std::cout << "Error loading enemy attack texture" << std::endl;
                return;
            }
            if (!moveTexture.loadFromFile("../resources/EnemyAnim/enemies-skeleton1_movement.png")) {
                std::cout << "Error loading enemy movement texture" << std::endl;
                return;
            }
            if (!deathTexture.loadFromFile("../resources/EnemyAnim/enemies-skeleton1_death.png")) {
                std::cout << "Error loading enemy death texture" << std::endl;
                return;
            }
            if (!damageTexture.loadFromFile("../resources/EnemyAnim/enemies-skeleton1_take_damage.png")) {
                std::cout << "Error loading enemy damage texture" << std::endl;
                return;
            }

            // Set initial texture
            sprite.setTexture(idleTexture);

            // Setup frame rectangles for each animation
            // These values might need adjustment based on your actual sprite sheet dimensions
            const int FRAME_WIDTH = 32;  // Adjust based on your sprite sheet
            const int FRAME_HEIGHT = 32; // Adjust based on your sprite sheet

            // Setup idle animation frames
            for (int i = 0; i < 6; i++) { // Adjust frame count as needed
                idleFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            }

            // Setup attack animation frames
            for (int i = 0; i < 9; i++) { // Adjust frame count as needed
                attackFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            }

            // Setup movement animation frames
            for (int i = 0; i < 10; i++) { // Adjust frame count as needed
                moveFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            }

            // Setup death animation frames
            for (int i = 0; i < 17; i++) { // Adjust frame count as needed
                deathFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            }

            // Setup damage animation frames
            for (int i = 0; i < 5; i++) { // Adjust frame count as needed
                damageFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            }
            hitbox.setFillColor(sf::Color::Transparent);
            hitbox.setOutlineColor(sf::Color::Red);
            hitbox.setOutlineThickness(1.f);

            sprite.setOrigin(sf::Vector2f(FRAME_WIDTH/2, FRAME_HEIGHT/2));
            sprite.setScale(sf::Vector2f(character_SCALE, character_SCALE));
            sprite.setPosition({200, 200});

            sf::Vector2f spritePos = sprite.getPosition();

            float hbWidth = FRAME_WIDTH * character_SCALE * 0.3f;
            float hbHeight = FRAME_HEIGHT * character_SCALE * 0.4f;

            hitbox.setSize({hbWidth, hbHeight});
            hitbox.setOrigin({hbWidth / 2.f, (hbHeight / 2.f)});
            hitbox.setPosition({spritePos.x, spritePos.y});
            clock.restart();
        }

        void update(float deltaTime) {
            if (isDead) {
                setAnimation(DEATH);
            }

            // Update animation frame
            if(clock.getElapsedTime() >= frameDuration) {
                std::vector<sf::IntRect>& currentFrames = getCurrentAnimationFrames();
                
                if(currentFrames.empty()) {
                    // If no frames are available, reset the clock and return
                    clock.restart();
                    return;
                }

                if(isDead && currentAnimation == DEATH) {
                    if(currentFrame < currentFrames.size() - 1) {
                        currentFrame++;
                    }
                    else if(!deathAnimationFinished) {
                        deathAnimationFinished = true;
                        deathTimer.restart();
                    }
                }
                else {
                    // Make sure we don't exceed the frame count
                    currentFrame = (currentFrame + 1) % currentFrames.size();
                }
                
                clock.restart();
            }
            
            sf::Vector2f movement = {0,0};
            float speed = 200.f;
            
            if(!isDead) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) || currentHealth <= 0) {
                    setAnimation(Enemy::DEATH);
                    isDead = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isAttacking) {
                    setAnimation(Enemy::ATTACK);
                    currentFrame = 0;
                    isAttacking = true;
                }
                // Handle damage on K key press
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
                    takeDamage(10.f); // Take 10 damage when K is pressed
                    isTakingDamage = true;
                }
                // Handle healing on H key press
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
                    heal(10.f); // Heal 10 HP when H is pressed
                }
                
                // Check for keyboard input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
                    movement = {-speed * deltaTime, 0.f};
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
                    movement = {speed * deltaTime, 0.f};
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
                    movement = {0.f, -speed * deltaTime};
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
                    movement = {0.f, speed * deltaTime};
                }

                if (movement != sf::Vector2f{0.f, 0.f}) {
                    if(!isAttacking){
                        move(movement.x, movement.y);
                        setAnimation(MOVE);
                    }
                } else {
                    if(isAttacking == true){
                        setAnimation(ATTACK);

                    } else{
                        setAnimation(IDLE);
                    }
                }
            }
            
            
            std::vector<sf::IntRect>& currentFrames = getCurrentAnimationFrames();
            
            // Apply the current frame to the sprite
            sprite.setTextureRect(currentFrames[currentFrame]);

            // Check if attack animation is finished
            if(isAttacking && currentAnimation == ATTACK && currentFrame >= getCurrentAnimationFrames().size() - 1) {
                isAttacking = false;
            }
            
        }
        
        void setAnimation(AnimationState animation) {
            
            if (animation != currentAnimation) {
                currentAnimation = animation;
                currentFrame = 0;
                
                // Set the appropriate texture for the animation
                switch (animation) {
                    case IDLE:
                        sprite.setTexture(idleTexture);
                        break;
                    case MOVE:
                        sprite.setTexture(moveTexture);
                        break;
                    case ATTACK:
                        sprite.setTexture(attackTexture);
                        break;
                    case DEATH:
                        sprite.setTexture(deathTexture);
                        break;
                    case TAKE_DAMAGE:
                        sprite.setTexture(damageTexture);
                        break;
                    default:
                        sprite.setTexture(idleTexture);
                        break;
                }
            }
        }

        void move(float x, float y) {
            sprite.move(sf::Vector2f(x, y));
            hitbox.move(sf::Vector2f(x,y));
            
            // Update direction based on movement
            if (x > 0) {
                setAnimation(MOVE);
                isFacingRight = true;
            } else if (x < 0) {
                setAnimation(MOVE);
                isFacingRight = false;
            } else if (y < 0) {
                setAnimation(MOVE);
            } else if (y > 0) {
                setAnimation(MOVE);
            }
            sprite.setScale(isFacingRight ? sf::Vector2f(character_SCALE,character_SCALE) : sf::Vector2f(-character_SCALE, character_SCALE));
        }

        void updateHealthBar() {
            // Get sprite bounds to position health bar above sprite
            sf::FloatRect spriteBounds = sprite.getGlobalBounds();
            
            // Position both bars above the sprite
            float xPos = spriteBounds.position.x + sprite.getGlobalBounds().size.x/4 ;
            float yPos = spriteBounds.position.y + 10.f; // 10 pixels above sprite
            
            healthBarBackground.setPosition(sf::Vector2f(xPos, yPos));
            healthBarFill.setPosition(sf::Vector2f(xPos, yPos));
            
            // Update health bar fill width based on current health percentage
            float healthPercentage = currentHealth / maxHealth;
            float currentWidth = healthBarBackground.getSize().x * healthPercentage;
            healthBarFill.setSize(sf::Vector2f(currentWidth, healthBarFill.getSize().y));
        }

        void takeDamage(float amount) {
            if (isDead) return;
            
            currentHealth = std::max(0.f, currentHealth - amount);
            setAnimation(TAKE_DAMAGE);
            
            if (currentHealth <= 0) {
                setAnimation(DEATH);
            }
        }

        void draw(sf::RenderWindow& window) {
            updateHealthBar();
            window.draw(sprite);
            window.draw(healthBarBackground);
            window.draw(healthBarFill);
            window.draw(hitbox);
        }

        sf::FloatRect getBounds() const {
            return sprite.getGlobalBounds();
        }
        
        sf::Vector2f getPosition() const {
            return sprite.getPosition();
        }

        // Add getter for current health
        float getHealth() const {
            return currentHealth;
        }

        // Add method to heal
        void heal(float amount) {
            if (isDead) return;
            currentHealth = std::min(maxHealth, currentHealth + amount);
        }

    private:
        std::vector<sf::IntRect>& getCurrentAnimationFrames() {
            switch (currentAnimation) {
                case IDLE: return idleFrames;
                case MOVE: return moveFrames;
                case ATTACK: return attackFrames;
                case DEATH: return deathFrames;
                case TAKE_DAMAGE: return damageFrames;
                default: return idleFrames;
            }
        }

        sf::Texture& getCurrentTexture() {
            switch (currentAnimation) {
                case IDLE: return idleTexture;
                case MOVE: return moveTexture;
                case ATTACK: return attackTexture;
                case DEATH: return deathTexture;
                case TAKE_DAMAGE: return damageTexture;
                default: return idleTexture;
            }
        }
};

// Example usage in main:
int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Enemy Animation");
    window.setFramerateLimit(60);  // Set frame rate to 60 FPS
    Enemy enemy;
    sf::Clock clock;
    enemy.load(0);
    
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        // Event handling
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        enemy.update(deltaTime.asSeconds());
        
        window.clear(sf::Color::Black);
        enemy.draw(window);
        window.display();
    }
    
    return 0;
}