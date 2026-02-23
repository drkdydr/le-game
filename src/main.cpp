#include "game.h"
#include <ncursesw/ncurses.h>


int main(){
     Game g; 
     g.initialize();
     g.start();
     
     // char ch ;
     // ch = getch();
     
     endwin();
}