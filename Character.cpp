#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

sf::Texture& getGlobalTexture(){
    static sf::Texture texture;
    if(!texture.loadFromFile("../resources/player.png")){
        std::cerr << "Error loading texture" << std::endl;

    }
    texture.setSmooth(false);
    return texture;
}

class Character {
private:
    sf::Sprite sprite;
    sf::Texture spriteSheet;
    std::vector<sf::IntRect> idleFrontFrames;
    std::vector<sf::IntRect> idleRightFrames;
    std::vector<sf::IntRect> idleBackFrames;
    std::vector<sf::IntRect> moveFrontFrames;
    std::vector<sf::IntRect> moveRightFrames;
    std::vector<sf::IntRect> moveBackFrames;
    std::vector<sf::IntRect> attackFrontFrames;
    std::vector<sf::IntRect> attackRightFrames;
    std::vector<sf::IntRect> attackBackFrames;
    std::vector<sf::IntRect> deathFrames;
    
    float animationSpeed;
    float currentFrame;
    int currentAnimation;
    int currentDirection;  // 0: front, 1: right, 2: back
    bool isFacingRight;

public:
    enum AnimationState {
        IDLE = 0,
        MOVE = 1,
        ATTACK = 2,
        DEATH = 3
    };

    enum Direction {
        FRONT = 0,
        RIGHT = 1,
        BACK = 2
    };

    Character() : animationSpeed(0.2f), currentFrame(0), currentAnimation(IDLE), currentDirection(FRONT), isFacingRight(true),sprite(getGlobalTexture()) {
        // Load sprite sheet
        if (!spriteSheet.loadFromFile("../resources/player.png")) {
            std::cout << "Error loading player sprite sheet" << std::endl;
            return;
        }
        sprite.setTexture(spriteSheet);

        const int FRAME_WIDTH = 48;
        const int FRAME_HEIGHT = 48;
        const int FRAMES_PER_ROW = 6;  // Number of frames in each row

        // Setup idle animations (rows 0-2)
        for (int i = 0; i < FRAMES_PER_ROW; i++) {
            idleFrontFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            idleRightFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            idleBackFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT * 2), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
        }

        // Setup move animations (rows 3-5)
        for (int i = 0; i < FRAMES_PER_ROW; i++) {
            moveFrontFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT * 3), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            moveRightFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT * 4), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            moveBackFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT * 5), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
        }

        // Setup attack animations (rows 6-8)
        for (int i = 0; i < 4; i++) {
            attackFrontFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT * 6), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            attackRightFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT * 7), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
            attackBackFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT * 8), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
        }

        // Setup death animation (row 9)
        for (int i = 0; i < 3; i++) {
            deathFrames.push_back(sf::IntRect(sf::Vector2i(i * FRAME_WIDTH, FRAME_HEIGHT * 9), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
        }

        sprite.setScale(sf::Vector2f(4.0f, 4.0f));
        sprite.setPosition(sf::Vector2f(400, 300));
    }

    void update(float deltaTime) {
        currentFrame += animationSpeed;
        
        std::vector<sf::IntRect>& currentFrames = getCurrentAnimationFrames();
        
        if (currentFrame >= currentFrames.size()) {
            if (currentAnimation == DEATH) {
                currentFrame = 0;
                // Stay on last death frame
                currentFrame = currentFrames.size() - 1;
            }
            
        }
        
        sprite.setTextureRect(currentFrames[static_cast<int>(currentFrame) % currentFrames.size()]);
    }

    void setAnimation(AnimationState animation, Direction direction) {
        if (animation != currentAnimation || direction != currentDirection) {
            currentAnimation = animation;
            currentDirection = direction;
            if(currentAnimation != IDLE)
                currentFrame = 0;
        }
    }

    void move(float x, float y) {
        sprite.move(sf::Vector2f(x, y));
        
        // Update direction based on movement
        if (x > 0) {
            setAnimation(MOVE, RIGHT);
            isFacingRight = true;
        } else if (x < 0) {
            setAnimation(MOVE, RIGHT);
            isFacingRight = false;
        } else if (y < 0) {
            setAnimation(MOVE, BACK);
        } else if (y > 0) {
            setAnimation(MOVE, FRONT);
        }
        sprite.setScale(isFacingRight ? sf::Vector2f(4.f,4.f) : sf::Vector2f(-4.f, 4.f));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
    
    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
    void setLastDirection(Direction dir){
        lastDirection = dir;
    }
    Direction getLastDirection() const{
        return lastDirection;
    }

private:
    std::vector<sf::IntRect>& getCurrentAnimationFrames() {
        switch (currentAnimation) {
            case IDLE:
                switch (currentDirection) {
                    case FRONT: return idleFrontFrames;
                    case RIGHT: return idleRightFrames;
                    case BACK: return idleBackFrames;
                }
            case MOVE:
                switch (currentDirection) {
                    case FRONT: return moveFrontFrames;
                    case RIGHT: return moveRightFrames;
                    case BACK: return moveBackFrames;
                }
            case ATTACK:
                switch (currentDirection) {
                    case FRONT: return attackFrontFrames;
                    case RIGHT: return attackRightFrames;
                    case BACK: return attackBackFrames;
                }
            case DEATH:
                return deathFrames;
        }
        return idleFrontFrames; // default case
    }
    Direction lastDirection = FRONT; // store last direction
};

// Example usage in main:

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Character Animation");
    Character character;
    sf::Clock clock;

    bool isDead = false;
    
    while (window.isOpen()) {
        
        sf::Time deltaTime = clock.restart();

        // Event handling
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // Handle death on L key press
                if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
                    character.setAnimation(Character::DEATH, Character::RIGHT);
                    isDead = true;
                    std::cout << "Dead" << std::endl;
                }
            }
            if (!isDead) {
            // Handle movement
                float moveX = 0.f;
                float moveY = 0.f;
                Character::Direction lastDirection = character.getLastDirection();
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
                    moveX = -2.0f; moveY = 0.f;
                    lastDirection = Character::RIGHT;
                    std::cout << "Moving left" << std::endl;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
                    moveX = 2.0f; moveY = 0.f;
                    lastDirection = Character::RIGHT;
                    std::cout << "Moving right" << std::endl;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
                    moveX = 0.f; moveY = -2.0f;
                    lastDirection = Character::BACK;
                    std::cout << "Moving back" << std::endl;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
                    moveX = 0.f; moveY = 2.0f;
                    lastDirection = Character::FRONT;
                    std::cout << "Moving front" << std::endl;
                }

                bool isAttacking = false;
                // Handle attack on Space key press
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && lastDirection == Character::RIGHT) {
                    character.setAnimation(Character::ATTACK, Character::RIGHT);
                    lastDirection = Character::RIGHT;
                    std::cout << "Attacking Right" << std::endl;
                    isAttacking = true;
                } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && lastDirection == Character::FRONT){
                    lastDirection = Character::FRONT;
                    std::cout << "Attacking Front" << std::endl;
                    isAttacking = true;
                    character.setAnimation(Character::ATTACK, Character::FRONT);
                } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && lastDirection == Character::BACK){
                    lastDirection = Character::BACK;
                    std::cout << "Attacking Back" << std::endl;
                    character.setAnimation(Character::ATTACK, Character::BACK);
                    isAttacking = true;
                }
                
                if (moveX != 0.f || moveY != 0.f) {
                    if(isAttacking){
                        character.setAnimation(Character::ATTACK, lastDirection);
                        character.setLastDirection(lastDirection);
                    } else{
                        character.move(moveX, moveY);
                        character.setAnimation(Character::MOVE, lastDirection);
                        character.setLastDirection(lastDirection);
                    }
                } else {
                    if(isAttacking){
                        character.setAnimation(Character::ATTACK, lastDirection);
                        character.setLastDirection(lastDirection);
                    } else{
                        character.setAnimation(Character::IDLE, character.getLastDirection());
                    }
                }
            }
        }

        character.update(deltaTime.asSeconds());
        
        window.clear(sf::Color(50, 50, 50));
        character.draw(window);
        window.display();
    }
    
    return 0;
}

