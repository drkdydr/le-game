#ifndef GAME_H
#define GAME_H

#include <cstring>
#include <ncurses.h>
#include <string>
#include <vector>
#include "entities.h"

int findDigits(int num);

void bold_box(WINDOW* w);

// struct block{
//   friend class Game;
//      WINDOW* win;
     
//      int width;
//      int height;
     
//      int abs_x;
//      int abs_y;
//      bool isVisible;
     
//      block(int h, int w, int y, int x): isVisible(false), height(h), width(w), abs_y(y), abs_x(x){
//           win = newwin(height,width,abs_y,abs_x);
//           // win = newpad(height,width);
//      }
// } ;


struct BUTTON {
     int default_width;
     int default_height;
     WINDOW* win;
     
     int rel_y;
     
     bool isSelected;
     
     std::string content;
     
     BUTTON(WINDOW* parent, char* cont, int begin_y);
     void drawButt();
     
};

class Game{
     std::vector<const char*> game_logo = {"$$\\         $$\\  $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\ ",
                                           "$$ |        $  |$$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|",
                                           "$$ | $$$$$$\\\\_/ $$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      ",
                                           "$$ |$$  __$$\\   $$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$$$$$\\    ",
                                           "$$ |$$$$$$$$ |  $$ |\\_$$ |$$  __$$ |$$ \\$$$  $$ |$$  __|   ",
                                           "$$ |$$   ____|  $$ |  $$ |$$ |  $$ |$$ |\\$  /$$ |$$ |      ",
                                           "$$ |\\$$$$$$$\\   \\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\\ ",
                                           "\\__| \\_______|   \\______/ \\__|  \\__|\\__|     \\__|\\________|",};
     
     
     std::vector<const char*> select_logo =   {" _____ _____ __    _____ _____ _____ _ ",
                                               "|   __|   __|  |  |   __|     |_   _|_|",
                                               "|__   |   __|  |__|   __|   --| | |  _ ",
                                               "|_____|_____|_____|_____|_____| |_| |_|"};
     
     void printLogo(std::vector<const char*> &logo, int y_idx);
     // 0 : for main-logo 1 : for select logo 
     // (if i want to add new logos i will switch id's data type to integer.)
     
     char* windowTitle;
     
     bool exitWanted = false;
     
     bool inMain = true;
     bool inSelect = false;
     bool inGame1 = false;
     bool inGame2 = false;
     bool inGame3 = false;
     
     
     // bool inSS = false; // (SpaceShooters) bu şekilde mi yapacağım emin değilim.
     
     int win_width= 75 , win_height = 18;
     int lastLINES, lastCOLS; // window size değişmiş mi kontrol için
     // block* gamewin;
     // block* selectionbox1;
     // block* selectionbox2;
     // block* selectionbox3;
    
     void resizeNotif();
     
     void handleMain(int input);
     void drawMain();
     WINDOW* mainwin = nullptr;
          // buttonları daha mainwin olmadan initialize ettiğim için seg fault yemişim.
          // BUTTON* startbutt = new BUTTON(mainwin,"START",10);
          // BUTTON* exitbutt = new BUTTON(mainwin,"EXIT",13);
          BUTTON* startbutt = nullptr;
          BUTTON* exitbutt = nullptr;
     
     void handleSelec(int input);
     void drawSelec();   
          // BUTTON* game1butt = new BUTTON(selecwin,"SPACE SHOOTERS",7);
          // BUTTON* game2butt = new BUTTON(selecwin,"DINO GAME",10);
          // BUTTON* game3butt = new BUTTON(selecwin,"COMING SOON",13);
          BUTTON* game1butt = nullptr; 
          BUTTON* game2butt = nullptr; 
          BUTTON* game3butt = nullptr;
          
     WINDOW* game1win = nullptr;
     WINDOW* game2win = nullptr;
     WINDOW* game3win = nullptr;
    
     // void alignButton(BUTTON* butt, int h, int w);
     bool isButtonsHidden;
     void hideButton(BUTTON* b);
     void alignButton(BUTTON* b);
     
     BUTTON* mainwinButtons[2] = {startbutt,exitbutt};
     BUTTON* selecwinButtons[3] = {game1butt,game2butt,game3butt};
     
    // void create_win(BLOCK* win); // create win and resize according stdscr
    // void handle_win(BLOCK); // handle already exist win size according stdscr
    
     void alignWin(); // bunun başına block constructor yaptığımda zaten iki fonksiyonu da elde edeceğim.
     
     
     public:
     void initialize();
     void start();
};


#endif