#ifndef DINOGAME_H
#define DINOGAME_H

#include <ncurses.h>
class DinoGame{
     public:
          const char* getName();
          void process(WINDOW* &win, int input);
          void print(WINDOW* &win);
          void reset();
};

#endif
