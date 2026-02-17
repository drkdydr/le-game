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

struct BLOCK {
     int width, height;
     int abs_x, abs_y;
     WINDOW* win;
};

struct BUTTON : public BLOCK{
     
     char* content;
     
     BUTTON(BLOCK* parent, char* cont, int begin_y){
          width = 31;
          height = 5;
          
          abs_x = (parent->width-width)/2;
          abs_y = begin_y;
          
          content = cont;
          
          win = subwin(parent->win, 5, 31, abs_y, abs_x);
     }
     
     void redraw(bool isBold){
          if (isBold){
               bold_box(this->win);
          }else{
               box(this->win,0,0);
               wrefresh(this->win);
          }
          mvwprintw(this->win,3,(width-strlen(content))/2,content);
          wrefresh(this->win);
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
     
     // block* gamewin;
     // block* selectionbox1;
     // block* selectionbox2;
     // block* selectionbox3;
     
     BLOCK* mainwin;
          BUTTON* startbutt = new BUTTON(mainwin,"START",10);
          BUTTON* exitbutt = new BUTTON(mainwin,"EXIT",13);
          
     BLOCK* selecwin;
          BUTTON* game1butt = new BUTTON(selecwin,"SPACE SHOOTERS",7);
          BUTTON* game2butt = new BUTTON(selecwin,"DINO GAME",10);
          BUTTON* game3butt = new BUTTON(selecwin,"COMING SOON",13);
          
     BLOCK* game1win;
     BLOCK* game2win;
     BLOCK* game3win;
     
     
     void drawCurrScr();
     // void sizeCheck(); // will check everytime and alert if size is changed
     //      void resize();
          void sizeAgain();
               void drawWin();
     
     // void hideBlk(* blk);

     public:
     void initialize();
     void start();
};


#endif