#include <SFML/Graphics.hpp>
#include <iostream>
#include "constants.cpp"

struct SpriteBounds{
    sf::Vector2f position; // position of the sprite
    sf::Vector2f size = {TILE_SIZE, TILE_SIZE}; // size of the sprite
};

class Map {
    private:
        sf::Texture mapTexture; // map tile texture
        int (*currentMap)[LEVEL_WIDTH]; // Pointer to the current map array
        int LevelNumber; // Current level number

        struct SpriteBounds mapsprites[LEVEL_WIDTH][LEVEL_HEIGHT];  // Array of map sprite bounds

    public:
        Map() : LevelNumber(0) {
            currentMap = MAPS[0];
        }

        SpriteBounds getMapTileSprite(int index_X , int index_Y){ // returns the sprite of the map tile at the given index
            return mapsprites[index_X][index_Y];  
        };

        void LoadMap(){
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
                    tempSprite.setPosition(sf::Vector2f((float)(x * TILE_SIZE * SCALE), (float)(y * TILE_SIZE * SCALE)));

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
        const int (*currentFloor)[LEVEL_WIDTH]; // Pointer to the current floor array
        int LevelNumber; // current level number

    public:
        Floor() : LevelNumber(0) {
            currentFloor = FLOORS[0];
        }

        void LoadFloor(const int level) { //    takes the level number and loads the corresponding level
            if (level >= 0 && level < NUMBER_OF_LEVELS) {
                LevelNumber = level;
                if ( LevelNumber == 0) {
                    if (!floorTexture.loadFromFile("../resources/Mini_Dungeon_Tileset/FREE/1_MiniDungeon_Tileset_Background1.png")) {
                        std::cout << "Texture didn't load successfully!\n";
                    } 
                    else { 
                        std::cout << "Texture loaded successfully!\n"; 
                    }
                    floorTexture.setSmooth(false);

                }
                if ( LevelNumber == 1) {
                    
                    floorTexture.setSmooth(false);

                }
                if ( LevelNumber == 2) {

                }
                currentFloor = FLOORS[LevelNumber];
            }
        }

        void Render(sf::RenderWindow& window) {

            sf::Sprite floorSprite(floorTexture);

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
        ~Floor(){}
};

