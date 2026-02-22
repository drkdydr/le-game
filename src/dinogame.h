#ifndef DINOGAME_H
#define DINOGAME_H

#include <ncurses.h>
class DinoGame{
     public:
          bool process(WINDOW* &win, int input);
          void print(WINDOW* &win);
          void reset();
};

#endif