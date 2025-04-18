#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "constants.cpp"

class Props{
    private:
        sf::Texture propTexture; // prop texture
        int (*currentProps)[LEVEL_WIDTH]; // Pointer to the prop layout
        sf::FloatRect propsprites[LEVEL_WIDTH][LEVEL_HEIGHT] = {sf::FloatRect({0,0},{TILE_SIZE, TILE_SIZE})}; // for each props, size and position, to calc collision 
        int LevelNumber;
        sf::RectangleShape door_hitbox;

    public:
        Props() : LevelNumber(0) {
            currentProps = PROPS[0];
        }
        bool isCollidable(int tile) const{
            return tile == 24 || tile == 31 || tile == 26; // return a coin, key or a door
        }
        int getPropID(int x, int y) const{ // returns the prop id at the given position
            return currentProps[y][x];
        }
        void collectTile(int x, int y){ // sets tile to an empty tile
            currentProps[y][x] = 0;
        }
        bool isDoor(sf::FloatRect rect) const{
            if(door_hitbox.getGlobalBounds().findIntersection(rect)){
                return true;
            }
            return false;
        }
        std::vector<sf::FloatRect> GetPropCollisionRects() const {
            std::vector<sf::FloatRect> collidables;
            for (int y = 0; y < LEVEL_HEIGHT; ++y) {
                for (int x = 0; x < LEVEL_WIDTH; ++x) {
                    if (isCollidable(PROPS[LevelNumber][y][x])) { // replace with relevant prop IDs
                        collidables.push_back(propsprites[x][y]);
                    }
                }
            }
            collidables.push_back(door_hitbox.getGlobalBounds()); // add door hitbox to the list of collidable rects 
            return collidables;
        }       

        void LoadProps(const int level){  // takes the level number and loads the corresponding props layout
            LevelNumber = level;

            if (!propTexture.loadFromFile("../resources/Dungeon_16x16_asset_pack/tileset.png")) {
                std::cout << "Props didn't load successfully!\n";
            } 
            else { 
                std::cout << "Props loaded successfully!\n";
            }
            propTexture.setSmooth(false);

            currentProps = PROPS[LevelNumber];
        }
        
        void Render(sf::RenderWindow& window){
            sf::Sprite tempSprite(propTexture);

            for (int y = 0; y < LEVEL_HEIGHT; ++y) {
                for (int x = 0; x < LEVEL_WIDTH; ++x) {
                    int proptile = abs(currentProps[y][x]);

                    if(PROPS[LevelNumber][y][x] != 0){
                        if(abs(PROPS[LevelNumber][y][x]) == 33){ // torch lateral wall
                            if(PROPS[LevelNumber][y][x] == 33){ // torch on the left
                                // calculate prop sprite used from tile set
                                int proptileX = (proptile % maptilesetColumns) * TILE_SIZE;
                                int proptileY = (proptile / maptilesetColumns) * TILE_SIZE;
        
                                sf::IntRect proptilerect({proptileX, proptileY}, {TILE_SIZE, TILE_SIZE});
        
                                tempSprite.setTextureRect(proptilerect);
                                tempSprite.setScale({SCALE, SCALE});
                                float deviation = 0;
                                if(LevelNumber == 2){ // move all prop tiles in level 2 slightly to the left 
                                    deviation = 7;
                                }

                                float posx = x * TILE_SIZE * SCALE + (TILE_SIZE * SCALE) / 3 - deviation;
                                float posy = y * TILE_SIZE * SCALE;

                                tempSprite.setPosition(sf::Vector2f(posx, posy));
                            } else{ // torch on the right
                                // calculate prop sprite used from tile set
                                int proptileX = (proptile % maptilesetColumns) * TILE_SIZE;
                                int proptileY = (proptile / maptilesetColumns) * TILE_SIZE;

                                sf::IntRect proptilerect({proptileX, proptileY}, {TILE_SIZE, TILE_SIZE});
                                
                                tempSprite.setTextureRect(proptilerect);
                                tempSprite.setScale({-SCALE, SCALE});

                                float torchx_displacement = (LevelNumber == 1 || LevelNumber == 0) ? (TILE_SIZE * SCALE) / 3 : 0 ;

                                float deviation = 0;
                                if(LevelNumber == 2){ // move all prop tiles in level 2 slightly to the left 
                                    deviation = 7;
                                }

                                float posx = (x + 1) * TILE_SIZE * SCALE - torchx_displacement - deviation;
                                float posy = y * TILE_SIZE * SCALE;

                                tempSprite.setPosition(sf::Vector2f(posx, posy));
                            }
                        }
                         else{
                            if(PROPS[LevelNumber][y][x] == 26){ // door

                                door_hitbox.setFillColor(sf::Color::Transparent);
                                door_hitbox.setOutlineColor(sf::Color::Red);
                                door_hitbox.setOutlineThickness(1.f);

                                // calculate prop sprite used from tile set
                                int proptileX = (proptile % maptilesetColumns) * TILE_SIZE;
                                int proptileY = (proptile / maptilesetColumns) * TILE_SIZE;
                                
                                
                                sf::IntRect proptilerect({proptileX, proptileY}, {TILE_SIZE, TILE_SIZE});
                                
                                tempSprite.setTextureRect(proptilerect);
                                tempSprite.setScale({SCALE, SCALE-1});
                                door_hitbox.setSize({TILE_SIZE, TILE_SIZE});
                                door_hitbox.setScale({SCALE, SCALE-1});
                                
                                float deviation = 0;
                                if(LevelNumber == 2){ // move all prop tiles in level 2 slightly to the left 
                                    deviation = 7;
                                }

                                float posx = x * TILE_SIZE * SCALE + (TILE_SIZE * SCALE) - deviation;
                                float posy = y * TILE_SIZE * (SCALE + 1);
                                
                                tempSprite.setPosition({posx, posy});
                                int offsety = 5; // offset to make door hitbox stickout from the wall
                                door_hitbox.setPosition({posx, posy + offsety});

                            } else{ // for a collidable prop
                                // calculate prop sprite used from tile set
                                int proptileX = (proptile % maptilesetColumns) * TILE_SIZE;
                                int proptileY = (proptile / maptilesetColumns) * TILE_SIZE;

                                sf::IntRect proptilerect({proptileX, proptileY}, {TILE_SIZE, TILE_SIZE});

                                tempSprite.setTextureRect(proptilerect);
                                tempSprite.setScale({SCALE, SCALE});

                                float deviation = 0;
                                if(LevelNumber == 1 || LevelNumber == 0){
                                    deviation = 1;
                                }
                                if(LevelNumber == 2){ // move all prop tiles in level 2 slightly to the left 
                                    deviation = 7;
                                }

                                float posx = x * TILE_SIZE * SCALE + (TILE_SIZE * SCALE) - deviation;
                                float posy = y * TILE_SIZE * SCALE;
                                tempSprite.setPosition(sf::Vector2f(posx, posy));  

                                propsprites[x][y].position = tempSprite.getGlobalBounds().position;
                                propsprites[x][y].size = tempSprite.getGlobalBounds().size;
                            }
                        }
                    } else{ continue; }

                    window.draw(tempSprite);
                    if(getPropID(x, y) == 26){
                        window.draw(door_hitbox);
                    } 
                }
            }            
        }

        ~Props(){}
};

class Map {
    private:
        sf::Texture mapTexture; // map tile texture
        int (*currentMap)[LEVEL_WIDTH]; // Pointer to the current map array
        int LevelNumber; // Current level number

        sf::FloatRect mapsprites[LEVEL_WIDTH][LEVEL_HEIGHT];  // Array of map sprite bounds for wall collisions

    public:
        Map() : LevelNumber(0) {
            currentMap = MAPS[0];
        }

        std::vector<sf::FloatRect> GetMapCollisionRects() const {
            std::vector<sf::FloatRect> collidables;
            for (int y = 0; y < LEVEL_HEIGHT; ++y) {
                for (int x = 0; x < LEVEL_WIDTH; ++x) {
                    if (MAPS[LevelNumber][y][x] != 16 && MAPS[LevelNumber][y][x] != 20) { 
                        collidables.push_back(mapsprites[x][y]);
                    }
                }
            }
            return collidables;
        }

        void LoadMap(const int level){  //    takes the level number and loads the corresponding map
            LevelNumber = level;
            if (!mapTexture.loadFromFile("../resources/Dungeon_16x16_asset_pack/tileset.png")) {
                std::cout << "Map didn't load successfully!\n";
            } 
            else { 
                std::cout << "Map loaded successfully!\n";
            }
            mapTexture.setSmooth(false);

            currentMap = MAPS[LevelNumber];
        }

        void Render(sf::RenderWindow& window){
            sf::Sprite tempSprite(mapTexture);

            for (int y = 0; y < LEVEL_HEIGHT; ++y) {
                for (int x = 0; x < LEVEL_WIDTH; ++x) {
                    int maptile = currentMap[y][x];

                    int maptileX = (maptile % maptilesetColumns) * TILE_SIZE;
                    int maptileY = (maptile / maptilesetColumns) * TILE_SIZE;

                    sf::IntRect maptilerect({maptileX, maptileY}, {TILE_SIZE, TILE_SIZE});

                    tempSprite.setTextureRect(maptilerect);
                    tempSprite.setScale({SCALE, SCALE});

                    float deviation = 0;
                    if(LevelNumber == 2){ // move all map tiles in level 2 to the left
                        deviation = 7;
                    }

                    float posx = x * TILE_SIZE * SCALE - deviation;
                    float posy = y * TILE_SIZE * SCALE;
                    tempSprite.setPosition(sf::Vector2f(posx, posy));

                    
                    mapsprites[x][y].position = tempSprite.getGlobalBounds().position;
                    mapsprites[x][y].size = tempSprite.getGlobalBounds().size;
                    

                    window.draw(tempSprite);
                }
            }            
        }

        ~Map(){}
};

class Floor {
    private:
        sf::Texture floorTexture; // floor tile texture
        sf::Texture lavaTexture; // lava tile texture
        const int (*currentFloor)[LEVEL_WIDTH]; // Pointer to the current floor array
        const int (*LavaPointer)[LEVEL_WIDTH]; // Pointer to the Lava array
        int LevelNumber; // current level number

        // Animated Lava Functionality
        sf::Clock lavaAnimClock;
        sf::Time lavaFrameDuration = sf::milliseconds(200); // 5 fps
        int currentlavaframe = 0;


        void LavaAnimUpdate(){
            if (lavaAnimClock.getElapsedTime() > lavaFrameDuration) {
                currentlavaframe = (currentlavaframe + 1) % lavatilesetColumns;
                lavaAnimClock.restart();
            }
        }
    public:
        Floor() : LevelNumber(0) {
            currentFloor = FLOORS[0];
            lavaAnimClock.start();
        }

        void LoadFloor(const int level) { //    takes the level number and loads the corresponding level
            if (level == 0 || level == 1) {
                LevelNumber = level;

                if (!floorTexture.loadFromFile("../resources/Mini_Dungeon_Tileset/FREE/1_MiniDungeon_Tileset_Background1.png")) {
                    std::cout << "Floor didn't load successfully!\n";
                } 
                else { 
                    std::cout << "Floor loaded successfully!\n"; 
                }
            }
            if(level == 2) {
                LevelNumber = level;
                if (!floorTexture.loadFromFile("../resources/Mini_Dungeon_Tileset/FREE/1_MiniDungeon_Tileset_Background1.png")) {
                    std::cout << "Floor didn't load successfully!\n";
                } 
                else { 
                    std::cout << "Floor loaded successfully!\n"; 
                }
                if (!lavaTexture.loadFromFile("../resources/Top_Down_Lava_Tileset_16x16_Free/FREE_TILESET_FILES/Environment(With_Animations)/Burning_Lava/spritesheet-burninglava.png")) {
                    std::cout << "Lava didn't load successfully!\n";
                } 
                else { 
                    std::cout << "Lava loaded successfully!\n"; 
                }
                LavaPointer = LAVA;
                lavaTexture.setSmooth(false);
            }
            floorTexture.setSmooth(false);
            
            
            currentFloor = FLOORS[LevelNumber];
        }

        void Render(sf::RenderWindow& window) {

            sf::Sprite floorSprite(floorTexture);
            sf::Sprite lavaSprite(lavaTexture);

            if(LevelNumber == 0 || LevelNumber == 1) {
                for (int y = 0; y < LEVEL_HEIGHT; ++y) {
                    for (int x = 0; x < LEVEL_WIDTH; ++x) {

                        int floortile = currentFloor[y][x];

                        // Calculate row and column in the tileset
                        int floortileX = (floortile % floortilesetColumns) * TILE_SIZE;
                        int floortileY = (floortile / floortilesetColumns) * TILE_SIZE;

                        sf::IntRect floortilerect({floortileX, floortileY}, {TILE_SIZE, TILE_SIZE});

                        floorSprite.setTextureRect(floortilerect);
                        floorSprite.setScale({SCALE, SCALE});
                        floorSprite.setPosition(sf::Vector2f((float)(x * TILE_SIZE * SCALE), (float)(y * TILE_SIZE * SCALE)));

                        window.draw(floorSprite);
                    }
                }
            }
            if(LevelNumber == 2){
                LavaAnimUpdate();
                for (int y = 0; y < LEVEL_HEIGHT; ++y) {
                    for (int x = 0; x < LEVEL_WIDTH; ++x) {
                        
                        int floortile = currentFloor[y][x]; // Floor tileID
                        int lavatile = LavaPointer[y][x]; // Lava tileID

    
                        // Calculate row and column in the tileset
                        int floortileX = (floortile % floortilesetColumns) * TILE_SIZE;
                        int floortileY = (floortile / floortilesetColumns) * TILE_SIZE;
    
                        sf::IntRect floortilerect({floortileX, floortileY}, {TILE_SIZE, TILE_SIZE});
    
                        floorSprite.setTextureRect(floortilerect);
                        floorSprite.setScale({SCALE, SCALE});
                        floorSprite.setPosition(sf::Vector2f((x * TILE_SIZE * SCALE), (y * TILE_SIZE * SCALE)));
    
                        window.draw(floorSprite);

                        if (lavatile != 3) { // if theres lava, 3 is empty
                            // Calculate row and column in the tileset
                            int lavaFrameWidth = TILE_SIZE * lavatilesetColumns; 
                            
                            int lavatileX = ((currentlavaframe) * lavaFrameWidth) + ((lavatile % lavatilesetColumns) * TILE_SIZE);
                            int lavatileY = (lavatile / lavatilesetColumns) * TILE_SIZE; // if lavatile values range above 6
            
                            sf::IntRect lavatilerRect({lavatileX, lavatileY}, {TILE_SIZE, TILE_SIZE});

                            lavaSprite.setTextureRect(lavatilerRect);
                            lavaSprite.setScale({SCALE, SCALE});
                            lavaSprite.setPosition(sf::Vector2f(x * TILE_SIZE * SCALE, y * TILE_SIZE * SCALE));

                            window.draw(lavaSprite);
                        }
                    }
                }
            }
        }
        ~Floor(){}
};

