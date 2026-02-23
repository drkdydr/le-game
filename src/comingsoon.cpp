// #include "comingsoon.h"
#include <cstring>
#include <ncurses.h>
#include "comingsoon.h"

bool ComingSoon::process(WINDOW* &win, int input){
     print(win);
     switch(input){
          case 27 : case 'q':
               return true;
               break;
          default :
               return false;
     }
}

void ComingSoon::print(WINDOW* &win){
     int curr_y = 6;
     int win_w = getmaxx(win);
     for(const char* line : comingsoon_logo)
          mvwprintw(win,curr_y++, (win_w - (static_cast<int>(strlen(line))))/2,"%s",line);
          
     mvwprintw(win,++curr_y, (win_w - (static_cast<int>(strlen(message))))/2,"%s",message);
     wnoutrefresh(win);
}

void ComingSoon::reset(){
}

const char* ComingSoon::getName(){
     return "COMING SOON";
}