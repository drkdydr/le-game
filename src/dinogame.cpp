#include "dinogame.h"
#include "game.h"
#include <ncurses.h>

void DinoGame::process(WINDOW* &win, int input){
     switch(input){
          case 27 : case 'q':
                Game::inPause = true;
               break;
          default :
               break;
     }
     print(win);
}

void DinoGame::print(WINDOW* &win){}

void DinoGame::reset(){
}

const char* DinoGame::getName(){
     return "DINO GAME";
}
