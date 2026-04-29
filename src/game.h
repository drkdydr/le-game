#ifndef GAME_H
#define GAME_H

#include <cstring>
#include <ncurses.h>
#include <string>
#include <vector>
#include "spaceshooters.h"
#include "dinogame.h"
#include "comingsoon.h"



int findDigits(int num); // helper function to find the number of digits for a number

void bold_box(WINDOW* w);

struct BUTTON {
     int default_width;
     int default_height;
     WINDOW* win;
     
     int rel_y;
     
     bool isSelected;
     bool isHidden;
     
     std::string content;
     
     BUTTON(WINDOW* parent, const char* cont, int begin_y);
     void drawButt();
     
};

const int win_width= 75, win_height = 18;

class Game{
     
     friend class SpaceShooters;
     friend class DinoGame;
     friend class ComingSoon;
    
     //MENU NAMES:
     const char* mainName = "MAIN MENU";
     const char* selecName = "SELECTION MENU";
     const char* pauseName = "PAUSED";
     const char* victoryName = "VICTORY";
     const char* gameoverName = "GAME OVER";
     
     //MENU LOGOS:
     std::vector<const char*> game_logo = {
          "$$\\         $$\\  $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\ ",
          "$$ |        $  |$$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|",
          "$$ | $$$$$$\\\\_/ $$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      ",
          "$$ |$$  __$$\\   $$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$$$$$\\    ",
          "$$ |$$$$$$$$ |  $$ |\\_$$ |$$  __$$ |$$ \\$$$  $$ |$$  __|   ",
          "$$ |$$   ____|  $$ |  $$ |$$ |  $$ |$$ |\\$  /$$ |$$ |      ",
          "$$ |\\$$$$$$$\\   \\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\\ ",
          "\\__| \\_______|   \\______/ \\__|  \\__|\\__|     \\__|\\________|"
     };
     
     
     std::vector<const char*> select_logo =   {
          " _____ _____ __    _____ _____ _____ _ ",
          "|   __|   __|  |  |   __|     |_   _|_|",
          "|__   |   __|  |__|   __|   --| | |  _ ",
          "|_____|_____|_____|_____|_____| |_| |_|"
     };
     
     std::vector<const char*> pause_logo = {                                     
          " _____ _____ _____ _____ _____ ____  ",
          "|  _  |  _  |  |  |   __|   __|    \\ ",
          "|   __|     |  |  |__   |   __|  |  |",
          "|__|  |__|__|_____|_____|_____|____/ "
     };
     
     std::vector<const char*> victory_logo = {
          " _____ _____ _____ _____ _____ _____ __ __ ", 
          "|  |  |     |     |_   _|     | __  |  |  |",
          "|  |  |-   -|   --| | | |  |  |    -|_   _|",
          " \\___/|_____|_____| |_| |_____|__|__| |_|  "
     };
     
     std::vector<const char*> gameover_logo = {                                                    
          " _____ _____ _____ _____    _____ _____ _____ _____ ",
          "|   __|  _  |     |   __|  |     |  |  |   __| __  |",
          "|  |  |     | | | |   __|  |  |  |  |  |   __|    -|",
          "|_____|__|__|_|_|_|_____|  |_____|\\___/|_____|__|__|"   
     };
     
     void printLogo(std::vector<const char*> &logo, int y_idx);
     // 0 : for main-logo 1 : for select logo 
     // (if i want to add new logos i will switch id's data type to integer.)
     
     
     const char* windowTitle;
     
     bool exitWanted = false;
     bool escDetected = false;
     
     static bool inMain;
     static bool inSelect;
     static bool inPause;
     static bool inGames;
     static bool inGame1;
     static bool inGame2;
     static bool inGame3;
     static bool inVictory;
     static bool inGameOver;
     
     int lastLINES, lastCOLS; // window size değişmiş mi kontrol için
     
     int score;
     int in;
    
     void resizeNotif();
     
     WINDOW* game1win = nullptr;
     WINDOW* game2win = nullptr;
     WINDOW* game3win = nullptr;
     
     void handleMain(int input);
     void drawMain();
          void printVer();
     WINDOW* mainwin = nullptr;
          BUTTON* startbutt = nullptr;
          BUTTON* exitbutt = nullptr;
     
     void handleSelec(int input);
     void drawSelec();   
          BUTTON* game1butt = nullptr; 
          BUTTON* game2butt = nullptr; 
          BUTTON* game3butt = nullptr;
          
     void handlePause(int input);
     void drawPause();
          BUTTON* resumebutt = nullptr;
          BUTTON* restartbutt = nullptr;
          BUTTON* quitbutt = nullptr;
          
     void drawScore();
          
     void handleVictory(int input);
     void drawVictory();
          // BUTTON* quitbutt = nullptr; //zaten olanları kullanıcaz.
          // BUTTON* restartbutton = nullptr;
          
     void handleGameOver(int input);
     void drawGameOver();
          
     // void alignButton(BUTTON* butt, int h, int w);
     void hideButton(BUTTON* b);
     void alignButton(BUTTON* b);
    
    std::vector<BUTTON*> allButtons;
    std::vector<BUTTON*> mainButtons;
    std::vector<BUTTON*> selecButtons;
    std::vector<BUTTON*> pauseButtons;
    std::vector<BUTTON*> victoryButtons;
    std::vector<BUTTON*> gameOverButtons;
    
    
    SpaceShooters* game1;
    // DinoGame* game2;
    ComingSoon* game2;
    ComingSoon* game3;
    
    // void create_win(BLOCK* win); // create win and resize according stdscr
    // void handle_win(BLOCK); // handle already exist win size according stdscr
    
     void alignWin(); // bunun başına block constructor yaptığımda zaten iki fonksiyonu da elde edeceğim.
     void handleEscape();
     
     public:
     void initialize();
     void start();

    // friend class SpaceShooters, DinoGame, ComingSoon; // bu şekilde c++26 ile gelmiş
    friend class SpaceShooters;
    friend class DinoGame;
    friend class ComingSoon;
};


#endif
