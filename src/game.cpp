#include "game.h"
#include <ncurses.h>
#include <string.h>

void Game::initialize(){
     if (initscr()) printf("Error: screen could not initialized.");
     clear();
     cbreak();
     noecho();
     
     // windows cannot be larger than stdscr
     if (COLS >= win_width && LINES >= win_height){
          gamewin = new block(win_height, win_width, (LINES - win_height)/2, (COLS - win_width)/2 );
     }else{
          if (COLS < win_width && LINES < win_height){
               gamewin = new block(LINES, COLS, 0, 0 );
          }else if (COLS < win_width){
               gamewin = new block(win_height, COLS, (LINES - win_height)/2, 0 );
          }else{
               gamewin = new block(LINES, win_width, 0, (COLS - win_width)/2 );
          }    
     }
}

void Game::start(){
     
     while(true){
          if (COLS < win_height || LINES < win_width){ // hep bu değerden küçük olduğu için sonsuz döngüye giriyor
               // sizeNotif'te kalacak ve eğer bu win_width ve win_height değeri düzelirse çıkacak
               sizeAgain();
          }
          break;
          
     }
     endwin();
}

int findDigits(int num){
     if (num < 0) return findDigits(-num);
     if (num < 10) return 1;
     return 1 + findDigits(num/10);
}

void Game::sizeAgain(){ // acaba daha iyi nasıl yazabilirdim?
          
     // ekrandaki her şeyi sil
     //windowlar üst üste binebiliyor
     
     const char* mesg1 = "Screen size is too small.";
     const char* mesg2 = "(It should 75 COLS width and 16 LINES height.)";
     
     int lastCOLS, lastLINES;
     
     clear();
     
     do{
          lastCOLS = COLS;
          lastLINES = LINES;
          if (LINES < win_height || COLS < win_width){
               
               if (LINES < win_height && COLS < win_width){
                    wresize(gamewin->win, LINES, COLS); //resize gamewin because windows cannot bigger than stdscr
               }else if (LINES < win_height){
                    wresize(gamewin->win, LINES, win_width); //resize gamewin because windows cannot bigger than stdscr
               }else{
                    wresize(gamewin->win, win_height, COLS); //resize gamewin because windows cannot bigger than stdscr
               }
               
               int curr_y = (LINES-2)/2;
               
               clear();
               mvprintw(curr_y++,(COLS-strlen(mesg1))/2,"%s",mesg1);
               mvprintw(curr_y++,(COLS-strlen(mesg2))/2,"%s",mesg2);
               mvprintw(curr_y,(COLS-(strlen("Current Size : COLS-> ,LINES-> ")+findDigits(COLS)+findDigits(LINES)))/2,"Current Size : COLS-> %d, LINES-> %d",COLS,LINES);
          }
     }while (lastCOLS != COLS || lastLINES != LINES); // work only if size is changed.
               
     
     // ekranı geri yükle
     
}

void Game::hideBlk(block* blk){
     blk->isVisible = true;
     wborder(blk->win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
     wrefresh(blk->win);
}