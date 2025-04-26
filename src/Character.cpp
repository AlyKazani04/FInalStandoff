#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "constants.cpp"
#include "levels.cpp"

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
        sf::Clock clock;
        sf::RectangleShape hitbox;
        sf::RectangleShape attack_hitbox;

        //sound variables
        std::vector<sf::SoundBuffer> attackBuffers;
        std::vector<sf::Sound> attackSound;
        sf::SoundBuffer deathBuffer;
        std::vector<sf::Sound> deathSound;
        std::vector<sf::SoundBuffer> walkBuffers;
        std::vector<sf::Sound> walkSound;
        sf::Clock walkSoundClock;
        sf::Time walkSoundCooldown = sf::milliseconds(300);
        int lastWalkSoundIndex = -1;

        // animation variables
        sf::Time frameDuration = sf::seconds(0.1f); // in seconds, 0.1secs; 10fps
        int currentFrame;
        int currentAnimation;
        int currentDirection;  // 0: front, 1: right, 2: back
        bool isFacingRight;
        bool isDead = false;
        bool isAttacking = false;
        
        int LevelNumber;

        // logic variables
        float Health = MAX_HEALTH;
        int Coins = 0;
        const float attackDamage = 20;
        bool KeyCollected = false;
        bool movetonextlevel = false;
        sf::Clock deathTimer;
        bool deathAnimationFinished = false;
        
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
        
        Character() : currentFrame(0), currentAnimation(IDLE), currentDirection(FRONT), isFacingRight(true), sprite(getGlobalTexture()) {}
        
        void Load(int level){
            
            // initialize character variables
            LevelNumber = level;
            Health = MAX_HEALTH;
            Coins = 0;
            KeyCollected = false;
            movetonextlevel = false;
            isDead = false;
            isAttacking = false;
            currentFrame = 0;
            currentAnimation = IDLE;
            currentDirection = FRONT;
            isFacingRight = true;
            lastDirection = FRONT;
            
            // intialize sound buffers
            attackBuffers.resize(3);
            attackSound.clear();
            walkBuffers.resize(3);
            walkSound.clear();
            deathSound.clear();
            

            for (int i = 0; i < 3; ++i) {
                std::string path = "../resources/Minifantasy_Dungeon_SFX/07_human_atk_sword_" + std::to_string(i + 1) + ".wav";
                if (attackBuffers[i].loadFromFile(path)) {
                    attackSound.emplace_back(attackBuffers[i]);
                }
            }
            for (int i = 0; i < 3; ++i) {
                std::string path = "../resources/Minifantasy_Dungeon_SFX/16_human_walk_stone_" + std::to_string(i + 1) + ".wav";
                if (walkBuffers[i].loadFromFile(path)) {
                    walkSound.emplace_back(walkBuffers[i]);
                }
            }
            
            if(deathBuffer.loadFromFile("../resources/Minifantasy_Dungeon_SFX/14_human_death_spin.wav")){
                deathSound.emplace_back(deathBuffer);
                deathSound[0].setVolume(20);
                deathSound[0].setPlayingOffset(sf::seconds(0.5f));
            };
            
            for(int i = 0; i < 3; i++){
                attackSound[i].setBuffer(attackBuffers[i]);
                attackSound[i].setVolume(50);
            }
            for (int i = 0; i < 3; i++) {
                walkSound[i].setBuffer(walkBuffers[i]);
                walkSound[i].setVolume(20);
            }
            
            // clear animation frames
            idleFrontFrames.clear();
            idleRightFrames.clear();
            idleBackFrames.clear();
            moveFrontFrames.clear();
            moveRightFrames.clear();
            moveBackFrames.clear();
            attackFrontFrames.clear();
            attackRightFrames.clear();
            attackBackFrames.clear();
            deathFrames.clear();

            // Load sprite sheet
            if (!spriteSheet.loadFromFile("../resources/player.png")) {
                std::cout << "Error loading player sprite sheet" << std::endl;
                return;
            } else{
                std::cout << "Player sprite sheet loaded" << std::endl;
            }
            sprite.setTexture(spriteSheet);

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
            attack_hitbox.setFillColor(sf::Color::Transparent);
            attack_hitbox.setOutlineColor(sf::Color::Red);
            attack_hitbox.setOutlineThickness(1.f);
            

            sprite.setOrigin(sf::Vector2f(FRAME_WIDTH/2, FRAME_HEIGHT/2));
            sprite.setScale(sf::Vector2f(character_SCALE, character_SCALE));
            sprite.setPosition(characterSpawns[LevelNumber]);

            sf::Vector2f spritePos = sprite.getPosition();
            
            float atthbWidth = FRAME_WIDTH * 0.4f;
            float atthbHeight = FRAME_HEIGHT * 0.2f;
            
            float atthboffsety = 30;
            attack_hitbox.setSize({atthbWidth, atthbHeight});
            attack_hitbox.setPosition({spritePos.x, spritePos.y + atthboffsety});
            
            float hbWidth = FRAME_WIDTH * character_SCALE * 0.3f;
            float hbHeight = FRAME_HEIGHT * character_SCALE * 0.1f;
            float hitboxyoffset = 62.f;
            // Position it centered at sprite position
            hitbox.setSize({hbWidth, hbHeight});
            hitbox.setOrigin({hbWidth / 2.f, hbHeight / 2.f});
            hitbox.setPosition({spritePos.x, spritePos.y + hitboxyoffset});
            clock.restart();
        }

        void update(float deltaTime, const std::vector<sf::FloatRect>& mapRects, Prop& prop, sf::RenderWindow& window) {

            if (isDead){
                setAnimation(DEATH, lastDirection);
            }

            if (clock.getElapsedTime() > frameDuration) {
                std::vector<sf::IntRect>& currentFrames = getCurrentAnimationFrames();

                if(isDead && currentAnimation == DEATH){
                    if(currentFrame < currentFrames.size() - 1){
                        currentFrame++;
                    }
                    else if(!deathAnimationFinished){
                        deathAnimationFinished = true;
                        deathTimer.restart();
                    }
                }
                else{
                    currentFrame = (currentFrame + 1) % FRAMES_PER_ROW;
                }
                clock.restart();
            }

            sf::Vector2f movement = {0 , 0};
            float speed = 200.f;

            const std::vector<sf::FloatRect> propRects = prop.GetPropCollisionRects();

            if (!isDead) {

                // Handle movement
                //Handle death on L key press
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::L) || Health <= 0) {
                    setAnimation(DEATH, RIGHT);
                    isDead = true;
                    deathSound[0].play();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
                    movement = {-speed * deltaTime, 0.f};
                    lastDirection = RIGHT;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
                    movement = {speed * deltaTime, 0.f};
                    lastDirection = RIGHT;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
                    movement = {0.f, -speed * deltaTime};
                    lastDirection = BACK;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
                    movement = {0.f, speed * deltaTime};
                    lastDirection = FRONT;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && !isAttacking) {
                    isAttacking = true;
                    setAnimation(ATTACK, lastDirection);
                    currentFrame = 0;

                    // play a random attack sound effect
                    int index = rand() % 3;
                    attackSound[index].play();
                }

                sf::FloatRect nexthitbox = hitbox.getGlobalBounds();
                nexthitbox.position.x += movement.x;
                nexthitbox.position.y += movement.y;

                // screen boundary
                sf::Vector2f windowsize = window.getView().getSize();
                sf::FloatRect screenbounds = {{0,0}, {windowsize.x, windowsize.y}};
                bool blocked = false;

                if(nexthitbox.position.x < 0 || nexthitbox.position.y - 40 < 0 ||
                    (nexthitbox.position.x + nexthitbox.size.x) > (screenbounds.position.x + screenbounds.size.x) || 
                    (nexthitbox.position.y + nexthitbox.size.y) > (screenbounds.position.y + screenbounds.size.y)){
                        blocked = true;
                }

                for(const auto& rect : mapRects) {
                    if(nexthitbox.findIntersection(rect)) {
                        blocked = true;
                        break;
                    }
                }
                if(!blocked){

                    for(const auto& rect : propRects) {
                        if(nexthitbox.findIntersection(rect)){
                            int tileX = rect.position.x / TILE_SIZE / SCALE;
                            int tileY = rect.position.y / TILE_SIZE / SCALE;
                            int propID = prop.getTileID(tileX, tileY);

                            if(propID == 24){ // check if its a coin
                                prop.collectTile(tileX, tileY);
                                ++Coins;
                            }
                            if(propID == 31){ // check if its a key
                                prop.collectTile(tileX, tileY);
                                KeyCollected = true;
                            }
                            if(prop.isDoor(rect) && KeyCollected == true){ // check if its a door and whether the key has been collected, also check if enemies are dead(todo)
                                movetonextlevel = true;
                            }
                            break;
                        }
                    }
                }
                if (movement != sf::Vector2f{0.f, 0.f}) {
                    if(!isAttacking && !blocked){
                        playWalkSound();
                        move(movement.x, movement.y);
                        setAnimation(MOVE, lastDirection);
                        setLastDirection(lastDirection);
                    } else if(!isAttacking){
                        setAnimation(IDLE, lastDirection);
                    }
                } else {
                    if(isAttacking == true){
                        setAnimation(ATTACK, lastDirection);
                        setLastDirection(lastDirection);

                    } else{
                        setAnimation(IDLE, getLastDirection());
                    }
                }
            }

            std::vector<sf::IntRect>& currentFrames = getCurrentAnimationFrames();
            
            sprite.setTextureRect(currentFrames[static_cast<int>(currentFrame) % currentFrames.size()]);

            if(isAttacking == true && currentAnimation == ATTACK && currentFrame >= currentFrames.size() - 1){
                isAttacking = false;
            }
        }

        
        void move(float x, float y) {
            sprite.move(sf::Vector2f(x,y));
            hitbox.move(sf::Vector2f(x,y));
            attack_hitbox.move(sf::Vector2f(x,y));
            
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
            sprite.setScale(isFacingRight ? sf::Vector2f(character_SCALE,character_SCALE) : sf::Vector2f(-character_SCALE, character_SCALE));
            attack_hitbox.setScale(isFacingRight ? sf::Vector2f(character_SCALE, character_SCALE) : sf::Vector2f(-character_SCALE, character_SCALE));
        }
        
        void draw(sf::RenderWindow& window) {
            window.draw(sprite);
            if(isAttacking) window.draw(attack_hitbox); // for debugging, delete on release
        }
        
        void playWalkSound() {
            if (walkSoundClock.getElapsedTime() > walkSoundCooldown) {
                int index;
                do {
                    index = rand() % 3;
                } while (index == lastWalkSoundIndex);
        
                lastWalkSoundIndex = index;
        
                if (walkSound[index].getStatus() != sf::SoundSource::Status::Playing) {
                    walkSound[index].play();
                }
        
                walkSoundClock.restart();
            }
        }

        // getters
        float getCurrentHealth(){
            return Health;
        }
        
        sf::FloatRect getBounds() const {
            return sprite.getGlobalBounds();
        }
        
        sf::FloatRect getHitboxBounds() const{
            return hitbox.getGlobalBounds();
        }
        
        void takeDamage(int dmg){
            sprite.setColor(sf::Color(200,0,0,80));
            Health -= dmg;
        }

        sf::Vector2f getPosition() const {
            return sprite.getPosition();
        }
        
        Direction getLastDirection() const{
            return lastDirection;
        }
        
        // return flags
        bool isPlayerDead(){ 
            return isDead && deathAnimationFinished && deathTimer.getElapsedTime().asSeconds() >= 1.5f;
        }
        
        bool movetoNextLevel(){
            return movetonextlevel;
        }
        
        
        
    private:
        // setters
        void setAnimation(AnimationState animation, Direction direction) {
            if (animation != currentAnimation || direction != currentDirection) {
                currentAnimation = animation;
                currentDirection = direction;
                
                currentFrame = 0;
            }
        }

        void setLastDirection(Direction dir){
            lastDirection = dir;
        }
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

