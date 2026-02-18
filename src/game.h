#ifndef GAME_H
#define GAME_H

#include <cstring>
#include <ncurses.h>
#include <string>
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
     int width;
     int height;
     WINDOW* win;
     
     std::string content;
     
     BUTTON(WINDOW* parent, char* cont, int begin_y){
          width = 31;
          height = 5;
          
          int parent_y, parent_x;
          getmaxyx(parent,parent_y,parent_x);
          
          int abs_x = (parent_x-width)/2;
          int abs_y = begin_y;
          
          content = cont;
          
          win = subwin(parent, 5, 31, abs_y, abs_x);
     }
     
     void redraw(bool isBold){
          if (isBold){
               wattron(win, A_BOLD);
               bold_box(win);
          }else{
               box(win,0,0);
               wrefresh(win);
          }
          const char* contch = content.c_str();
          mvwprintw(win,3,(width-static_cast<int>(content.size()))/2,content.c_str());
          wrefresh(win);
     }
};

class Game{
     const char game_logo[8][60] = {"$$\\         $$\\  $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\ ",
                                    "$$ |        $  |$$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|",
                                    "$$ | $$$$$$\\\\_/ $$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      ",
                                    "$$ |$$  __$$\\   $$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$$$$$\\    ",
                                    "$$ |$$$$$$$$ |  $$ |\\_$$ |$$  __$$ |$$ \\$$$  $$ |$$  __|   ",
                                    "$$ |$$   ____|  $$ |  $$ |$$ |  $$ |$$ |\\$  /$$ |$$ |      ",
                                    "$$ |\\$$$$$$$\\   \\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\\ ",
                                    "\\__| \\_______|   \\______/ \\__|  \\__|\\__|     \\__|\\________|",};
     
     const char select_logo[4][40] =   {" _____ _____ __    _____ _____ _____ _ ",
                                        "|   __|   __|  |  |   __|     |_   _|_|",
                                        "|__   |   __|  |__|   __|   --| | |  _ ",
                                        "|_____|_____|_____|_____|_____| |_| |_|"};
     
     bool inMain = true;
     bool inSelect = false;
     // bool inSS = false; // (SpaceShooters) bu şekilde mi yapacağım emin değilim.
     
     int win_width= 75 , win_height = 16;
     int lastLINES, lastCOLS; // window size değişmiş mi kontrol için
     // block* gamewin;
     // block* selectionbox1;
     // block* selectionbox2;
     // block* selectionbox3;
    
    WINDOW* currwin = nullptr;
     
     void handleMain(int input);
     WINDOW* mainwin = nullptr;
          // buttonları daha mainwin olmadan initialize ettiğim için seg fault yemişim.
          // BUTTON* startbutt = new BUTTON(mainwin,"START",10);
          // BUTTON* exitbutt = new BUTTON(mainwin,"EXIT",13);
          BUTTON* startbutt = nullptr;
          BUTTON* exitbutt = nullptr;
          
     void handleSelec(int input);
     WINDOW* selecwin = nullptr;
          // BUTTON* game1butt = new BUTTON(selecwin,"SPACE SHOOTERS",7);
          // BUTTON* game2butt = new BUTTON(selecwin,"DINO GAME",10);
          // BUTTON* game3butt = new BUTTON(selecwin,"COMING SOON",13);
          BUTTON* game1butt = nullptr; 
          BUTTON* game2butt = nullptr; 
          BUTTON* game3butt = nullptr;
          
     WINDOW* game1win = nullptr;
     WINDOW* game2win = nullptr;
     WINDOW* game3win = nullptr;
    
    // void create_win(BLOCK* win); // create win and resize according stdscr
    // void handle_win(BLOCK); // handle already exist win size according stdscr
    
     void handleWin(WINDOW* win);
          void alignWin(WINDOW* win); // bunun başına block constructor yaptığımda zaten iki fonksiyonu da elde edeceğim.
     
     void drawCurrScr();
          void tooSmall();
               void drawWin();
     
     // void hideBlk(* blk);

     public:
     void initialize();
     void start();
};


#endif