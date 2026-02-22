#include "dinogame.h"
#include <ncurses.h>

bool DinoGame::process(WINDOW* &win, int input){
     switch(input){
          case 27 : case 'q':
               return true;
               break;
          default :
               return false;
     }
     print(win);
}

void DinoGame::print(WINDOW* &win){}

void DinoGame::reset(){
}