//THIS IS THE ENEMY CLASS THAT DEALS WITH THE ENEMY'S BEHAVIOR AND ANIMATION

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

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

    // Health bar shapes
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarFill;
    float maxHealth;
    float currentHealth;

    std::vector<sf::IntRect> idleFrames;
    std::vector<sf::IntRect> attackFrames;
    std::vector<sf::IntRect> moveFrames;
    std::vector<sf::IntRect> deathFrames;
    std::vector<sf::IntRect> damageFrames;
    
    float animationSpeed;
    float currentFrame;
    int currentAnimation;
    bool isFacingRight;
    bool isDead;
    bool isAttacking;
    bool isTakingDamage;

public:
    enum AnimationState {
        IDLE = 0,
        MOVE = 1,
        ATTACK = 2,
        DEATH = 3,
        TAKE_DAMAGE = 4
    };

    Enemy() : animationSpeed(0.2f), currentFrame(0), currentAnimation(IDLE), 
              isFacingRight(true), isDead(false), isAttacking(false), isTakingDamage(false),
              sprite(getGlobalTexture()),
              maxHealth(100.f), currentHealth(100.f) {
        
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

        sprite.setScale(sf::Vector2f(4.0f, 4.0f));
        sprite.setPosition(sf::Vector2f(400, 300));
    }

    void update(float deltaTime) {
        currentFrame += animationSpeed;
        
        std::vector<sf::IntRect>& currentFrames = getCurrentAnimationFrames();
        sf::Texture& currentTexture = getCurrentTexture();
        
        if (currentFrame >= currentFrames.size()) {
            if (currentAnimation == DEATH) {
                currentFrame = currentFrames.size() - 1;
                isDead = true;
            } else if (currentAnimation == ATTACK) {
                currentFrame = 0;
                isAttacking = false;
                setAnimation(IDLE);
            } else if (currentAnimation == TAKE_DAMAGE) {
                currentFrame = 0;
                isTakingDamage = false;
                setAnimation(IDLE);
            } else {
                currentFrame = 0;
            }
        }
        
        sprite.setTexture(currentTexture);
        sprite.setTextureRect(currentFrames[static_cast<int>(currentFrame) % currentFrames.size()]);
    }

    void setAnimation(AnimationState animation) {
        if (isDead) return;
        
        if (animation != currentAnimation) {
            currentAnimation = animation;
            currentFrame = 0;
            
            switch (animation) {
                case ATTACK:
                    isAttacking = true;
                    break;
                case TAKE_DAMAGE:
                    isTakingDamage = true;
                    break;
                case DEATH:
                    isDead = true;
                    break;
                default:
                    break;
            }
        }
    }

    void move(float x, float y) {
        if (isDead) return;
        
        sprite.move(sf::Vector2f(x, y));
        
        if (x != 0 || y != 0) {
            setAnimation(MOVE);
            isFacingRight = (x > 0);
            sprite.setScale(isFacingRight ? sf::Vector2f(4.f, 4.f) : sf::Vector2f(-4.f, 4.f));
        } else {
            setAnimation(IDLE);
        }
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
    
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        // Event handling
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            // Handle attack on Space key press
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    enemy.setAnimation(Enemy::ATTACK);
                }
                // Handle death on L key press
                if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
                    enemy.setAnimation(Enemy::DEATH);
                }
                // Handle damage on K key press
                if (keyPressed->scancode == sf::Keyboard::Scancode::K) {
                    enemy.takeDamage(20.f); // Take 20 damage when K is pressed
                }
                // Handle healing on H key press
                if (keyPressed->scancode == sf::Keyboard::Scancode::H) {
                    enemy.heal(10.f); // Heal 10 HP when H is pressed
                }
            }
        }

        // Handle movement
        float moveX = 0.0f;
        float moveY = 0.0f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) moveX = -2.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) moveX = 2.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) moveY = -2.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) moveY = 2.0f;
        
        enemy.move(moveX, moveY);
        enemy.update(deltaTime.asSeconds());
        
        window.clear(sf::Color::Black);
        enemy.draw(window);
        window.display();
    }
    
    return 0;
}