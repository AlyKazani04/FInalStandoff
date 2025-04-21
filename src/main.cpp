#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "levels.cpp"
#include "constants.cpp"
#include "sounds.cpp"
#include "fullscreen.cpp"
#include "timer.cpp"
#include "Character.cpp"

enum GameScreen{MENU, LEVEL1, LEVEL2, LEVEL3, DEATHMATCH, PAUSE, GAMEOVER, CREDITS};

int main(){

    // INITIALIZE GAME
    GameScreen currentScreen = LEVEL1; // Show Menu first
    GameScreen prevScreen = currentScreen;
    Floor floor;
    Map map;
    Prop prop;
    Character player;
    Character player2; // for deathmatch
    BackGroundMusic bgm;
    Timer timer;
    sf::Clock clock;

    int LevelNumber = 0;
    bool isFullScreen = true;
    bool isPause = false;

    createWindow(isFullScreen); // Create window in fullscreen mode
    floor.Load(LevelNumber);       // Game Starts at Level 0, then 1, then finally, 2
    map.Load(LevelNumber);            // Walls
    prop.Load(LevelNumber);          // Props
    bgm.LoadMusic(LevelNumber);            // Music
    timer.LoadTimer(window, LevelNumber);   // Timer
    player.Load(LevelNumber);                // Player
    
    window.setFramerateLimit(60);  // Max FrameRate set to 60 
    int framecounter = 0;
    bgm.play();


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
                if(isPause == false){
                    isPause = true;
                    timer.pause();
                    prevScreen = currentScreen;
                    currentScreen = PAUSE;
                } else{
                    isPause = false;
                    timer.unpause();
                    currentScreen = prevScreen;
                }
            }
        }
        
        { // UPDATE
            switch(currentScreen){
                case MENU:
                    // if(choice of selecting play ){
                    //     currentScreen = LEVEL1;
                    // }
                    // break;
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
                        player.update(deltaTime, map.GetMapCollisionRects(), prop);
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
                        player.update(deltaTime, map.GetMapCollisionRects(), prop);

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
                        player.update(deltaTime, map.GetMapCollisionRects(), prop);

                        if(player.isPlayerDead() || timer.isTimeUp()){
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
                    // do nothing
                    break;
                case DEATHMATCH:
                    break;
                case GAMEOVER:
                    break;
                case CREDITS:
                    break; 
            }
            
        }
        
        {// DRAW
            window.clear();     // clearing the window each frame
            switch(currentScreen){
                case MENU: // draw menu stuff here

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
                    break;
                case DEATHMATCH:
                    break;
                case GAMEOVER:
                    break;
                case CREDITS:
                    break; 
                default:
                    window.close();
                    return 0;
            }
            window.display();   // displaying the window (important)
        }

        // DEINITIALIZE (LOOP), if needed

    }
    // DEINITIALIZE GAME


        return 0;
}