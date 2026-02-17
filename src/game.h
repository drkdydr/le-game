#ifndef GAME_H
#define GAME_H

#include <ncurses.h>

int findDigits(int num);

struct block{
  friend class Game;
     WINDOW* win;
     
     int width;
     int height;
     
     int abs_x;
     int abs_y;
     bool isVisible;
     
     block(int h, int w, int y, int x): isVisible(false), height(h), width(w), abs_y(y), abs_x(x){
          win = newwin(height,width,abs_y,abs_x);
          // win = newpad(height,width);
     }
} ;

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
     
     block* gamewin;
     block* selectionbox1;
     block* selectionbox2;
     block* selectionbox3;
     
     void drawCurrScr();
     // void sizeCheck(); // will check everytime and alert if size is changed
     //      void resize();
          void sizeAgain();
               void drawWin();
     
     void hideBlk(block* blk);

     public:
     void initialize();
     void start();
};


#endif