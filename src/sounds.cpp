#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "constants.cpp"

enum class FadeState {
    None,
    FadeOut,
    FadeIn
};



class BackGroundMusic{
    private:
        sf::Music music;
        int LevelNumber;
    public:
        BackGroundMusic() : LevelNumber(0){}
        bool LoadMusic(int level){ // Load music from file
            LevelNumber = level;
            if(!music.openFromFile(filepath[level])){ // open file
                return false;
            }else{
                return true;
            }
        }
        void play(){ // play music
            
            sf::Music::TimeSpan looptime;

            if(LevelNumber == 0){ // set looping for bgm 1
                looptime.offset = sf::seconds(0.1);
                looptime.length = sf::seconds(16.8);
                music.setLoopPoints(looptime);
                music.setPlayingOffset(sf::seconds(0.2));
            }
            if(LevelNumber == 1){ // set looping for bgm 2
                looptime.offset = sf::seconds(1.10);
                looptime.length = sf::seconds(51.09);
                music.setLoopPoints(looptime);
                music.setPlayingOffset(sf::seconds(50));
            }

            music.setLooping(true);
            music.play();

        }
        void stop(){ // stop music
            music.stop();
        }
        void setVolume(int level = 70){ // set the volume level
            music.setVolume(level);
            // std::cout<<"Volume: "<<music.getVolume()<<std::endl;
        }
};
