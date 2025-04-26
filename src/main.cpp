#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "levels.cpp"
#include "constants.cpp"
#include "sounds.cpp"
#include "fullscreen.cpp"
#include "timer.cpp"
#include "Character.cpp"
#include "ScreenManager.hpp"
#include "ScreenManager.cpp"

enum GameScreen{MENU, LEVEL1, LEVEL2, LEVEL3, DEATHMATCH, PAUSE, GAMEOVER, CREDITS};

int main(){

    // INITIALIZE GAME
    GameScreen currentScreen = MENU; // Show Menu first
    GameScreen prevScreen = currentScreen;
    Floor floor;
    Map map;
    Prop prop;
    Character player;
    Character player2; // for deathmatch
    MenuMusic menubgm;
    BackGroundMusic bgm;
    Timer timer;
    sf::Clock clock;

    int LevelNumber = 0;
    bool isFullScreen = true;
    bool isPause = false;

    createWindow(isFullScreen); // Create window in fullscreen mode
    ScreenManager screenHandle;
    screenHandle.initialize(window);
    floor.Load(LevelNumber);       // Game Starts at Level 0, then 1, then finally, 2
    map.Load(LevelNumber);            // Walls
    prop.Load(LevelNumber);          // Props
    bgm.LoadMusic(LevelNumber);            // Music
    timer.LoadTimer(window, LevelNumber);   // Timer
    player.Load(LevelNumber);                // Player
    
    window.setFramerateLimit(60);  // Max FrameRate set to 60 
    int framecounter = 0;
    menubgm.LoadMusic();
    menubgm.play();


    while (window.isOpen()){
        // INITIALIZE (LOOP)
        framecounter++; // count frames...
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){ 
                bgm.stop(); // stop the music

                window.close();
            }
            if(event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape && isFullScreen == true){ // toggle windowed mode
                isFullScreen = false;
                createWindow(isFullScreen);
            }
            if(event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::F11 && isFullScreen == false){ // toggle fullscreen mode
                isFullScreen = true;
                createWindow(isFullScreen);
            }
            if(event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::P){
                if(currentScreen == LEVEL1 || currentScreen == LEVEL2 || currentScreen == LEVEL3){
                    if(!isPause){
                        isPause = true;
                        timer.pause();
                        prevScreen = currentScreen;
                        currentScreen = PAUSE;
                    }else if(currentScreen == PAUSE){
                        isPause = false;
                        timer.unpause();
                        currentScreen = prevScreen;
                    }
                }
            }
        }
        
        { // UPDATE
            switch(currentScreen){
                case MENU: // process menu events here
                {
                    bool startgame = false, exitgame = false;
                    screenHandle.handleStartScreenInput(window, startgame, exitgame);
                    if(startgame){
                        menubgm.stop();
                        currentScreen = LEVEL1;
                        bgm.LoadMusic(LevelNumber);
                        bgm.play();
                    } else if(exitgame){
                        window.close();
                    }  
                }
                    break;
                case LEVEL1:
                    if(player.movetoNextLevel() == false){

                        float deltaTime = 0.f;
                        if(currentScreen != PAUSE){
                            deltaTime = clock.restart().asSeconds(); // get the time since last frame in seconds
                        } else{
                            clock.restart();
                        }
                        timer.update(window);
                        if(timer.isTimeUp()){
                            timer.reset();
                        }
                        player.update(deltaTime, map.GetMapCollisionRects(), prop, window);
                        if(player.isPlayerDead() || timer.isTimeUp()){
                            currentScreen = GAMEOVER;
                        }
                    } else{
                        LevelNumber = 1;
                        floor.Load(LevelNumber);       
                        map.Load(LevelNumber);           
                        prop.Load(LevelNumber);        
                        timer.LoadTimer(window, LevelNumber);
                        player.Load(LevelNumber);
                        currentScreen = LEVEL2;
                    }
                    break;
                case LEVEL2:
                    if(player.movetoNextLevel() == false){
                        float deltaTime = 0.f;
                        if(currentScreen != PAUSE){
                            deltaTime = clock.restart().asSeconds(); // get the time since last frame in seconds
                        } else{
                            clock.restart();
                        }
                        timer.update(window);
                        if(timer.isTimeUp()){
                            timer.reset();
                        }
                        player.update(deltaTime, map.GetMapCollisionRects(), prop, window);

                        if(player.isPlayerDead() || timer.isTimeUp()){
                            currentScreen = GAMEOVER;
                        }
                    } else{
                        LevelNumber = 2;
                        bgm.stop();
                        floor.Load(LevelNumber);       
                        map.Load(LevelNumber);           
                        prop.Load(LevelNumber);        
                        bgm.LoadMusic(LevelNumber);         
                        timer.LoadTimer(window, LevelNumber);
                        player.Load(LevelNumber);
                        bgm.play();
                        currentScreen = LEVEL3;
                    }
                    break;
                case LEVEL3:
                    if(player.movetoNextLevel() == false){
                        float deltaTime = 0.f;
                        if(currentScreen != PAUSE){
                            deltaTime = clock.restart().asSeconds(); // get the time since last frame in seconds
                        } else{
                            clock.restart();
                        }
                        timer.update(window);
                        if(timer.isTimeUp()){
                            timer.reset();
                        }
                        player.update(deltaTime, map.GetMapCollisionRects(), prop, window);

                        if(player.isPlayerDead() || timer.isTimeUp()){
                            bgm.stop();
                            currentScreen = GAMEOVER;
                        }
                    } else{
                        LevelNumber = 0;
                        bgm.stop();
                        floor.Load(LevelNumber);       
                        map.Load(LevelNumber);           
                        prop.Load(LevelNumber);        
                        bgm.LoadMusic(LevelNumber);         
                        timer.LoadTimer(window, LevelNumber);
                        player.Load(LevelNumber);
                        bgm.play();
                        currentScreen = LEVEL1;
                    }
                    break;
                case PAUSE:
                {
                    bool resumegame = false, exitgame = false;
                    screenHandle.handlePauseScreenInput(window, resumegame,exitgame);
                    if(resumegame){
                        isPause = false;
                        timer.unpause();
                        currentScreen = prevScreen;
                    } else if(exitgame){
                        window.close();
                    }
                }
                    break;
                case DEATHMATCH:
                    break;
                case GAMEOVER:
                    break;
                case CREDITS:
                    break;
                default:
                    bgm.stop();
                    window.close();
                    return 0;
            }
            
        }
        
        {// DRAW
            window.clear(sf::Color::Black);     // clearing the window each frame
            switch(currentScreen){
                case MENU: // draw menu stuff here
                    screenHandle.renderStartScreen(window);
                    break;
                case LEVEL1:
                    floor.Render(window); // rendering the level
                    map.Render(window); // rendering the map
                    prop.Render(window); // rendering the props
                    timer.render(window); // render the timer
                    player.draw(window); // rendering the player
                    break;
                case LEVEL2:
                    floor.Render(window); // rendering the level
                    map.Render(window); // rendering the map
                    prop.Render(window); // rendering the props
                    timer.render(window); // render the timer
                    player.draw(window); // rendering the player
                    break;
                case LEVEL3:
                    floor.Render(window); // rendering the level
                    map.Render(window); // rendering the map
                    prop.Render(window); // rendering the props
                    timer.render(window); // render the timer
                    player.draw(window); // rendering the player
                    break;
                case PAUSE:
                    if(prevScreen == LEVEL1 || prevScreen == LEVEL2 || prevScreen == LEVEL3) {
                        floor.Render(window); // rendering the level
                        map.Render(window); // rendering the map
                        prop.Render(window); // rendering the props
                        timer.render(window); // render the timer
                        player.draw(window); // rendering the player
                    }
                    screenHandle.renderPauseScreen(window);
                    break;
                case DEATHMATCH:
                    break;
                case GAMEOVER:
                    break;
                case CREDITS:
                    break; 
            }
            window.display();   // displaying the window (important)
        }

        // DEINITIALIZE (LOOP), if needed

    }
    // DEINITIALIZE GAME


        return 0;
}