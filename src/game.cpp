#include "game.h"
#include <locale.h> //for setlocale
#include <cstdlib>
#include <ncurses.h>
#include <ncursesw/ncurses.h>
#include <string.h>
#include <algorithm>

// void Game::resizeBlock(BLOCK* blk){
//      // windows cannot be larger than stdscr
//      if (COLS >= win_width && LINES >= win_height){
//           // mainwin = new BLOCK(win_height, win_width, (LINES - win_height)/2, (COLS - win_width)/2 );
//           wresize(blk->win,win_height,win_width);
//           mvwin(blk->win,)
//      }else{
//           if (COLS < win_width && LINES < win_height){
//                gamewin = new block(LINES, COLS, 0, 0 );
//           }else if (COLS < win_width){
//                gamewin = new block(win_height, COLS, (LINES - win_height)/2, 0 );
//           }else{
//                gamewin = new block(LINES, win_width, 0, (COLS - win_width)/2 );
//           }    
//      }
// }

void Game::initialize(){
     
     setlocale(LC_ALL, ""); //for unicode char display
     
     initscr();
     cbreak();
     noecho();
     curs_set(0); // makes cursor invisible
}

void Game::start(){
     
     inMain = true;
     
     if (mainwin == nullptr){
          alignWin(mainwin);
     }
     
     currwin = mainwin;
     
     while(true){
          int ch = getch();
          if (ch == KEY_RESIZE){
               alignWin(currwin); //seg fault bebeğim!!!
          }

          // if (inMain){
          //      handleMain(ch);
          // }
          // if (inSelect){
          //      handleSelec(ch);
          // }
          
          
     }
     
     endwin();
}

void Game::alignWin(WINDOW* win){
     
     if (win == nullptr){
          currwin = newwin(1,1,0,0);
     }
     
     if(lastLINES != LINES || lastCOLS != COLS){
          
          lastLINES = LINES;
          lastCOLS = COLS;
          
          if (COLS >= win_width && LINES >= win_height){
               wclear(currwin);
               wresize(win,win_height,win_width);
               mvwin(win,(LINES-win_height)/2,(COLS-win_width)/2);
               box(win,0,0);
               wrefresh(win);
          }else{
               // if (COLS < win_width && LINES < win_height){ // hem width hem height yetersiz
               //      wresize(win,LINES,COLS);
               //      mvwin(win,0,0);
               // }else if (COLS < win_width){ // width yetersiz
               //      wresize(win,win_height,COLS);
               //      mvwin(win,(LINES-win_height)/2,0);
               // }else{ // height yetersiz
               //      wresize(win,LINES,win_width);
               //      mvwin(win,0,(COLS-win_width)/2);
               // }   
              
              int h = std::min(LINES, win_height);
              int w = std::min(COLS, win_height);
              
              wresize(win,h,w);
              mvwin(win, (LINES-h)/2, (COLS-w)/2);
              box(win,0,0);
              wrefresh(win);
               
              tooSmall();
          }
     }
     
}

void Game::handleMain(int input){
     if (startbutt == nullptr){
          startbutt = new BUTTON(mainwin,"START",10);
          exitbutt = new BUTTON(mainwin,"EXIT",13);
     }
}

void Game::handleSelec(int input){
}

int findDigits(int num){
     if (num < 0) return findDigits(-num);
     if (num == 0) return 0;
     return 1 + findDigits(num/10);
}

void Game::tooSmall(){ // acaba daha iyi nasıl yazabilirdim?
     //sorun şu an ekran küçülünce otomatik olarak uyarı moduna geçiyor fakat hem geri çıkmıyor hem de mod içinde güncellenmiyor.
     //difference detecting kısmında bir sıkıntı olabilir.
          
     // ekrandaki her şeyi sil
     //windowlar üst üste binebiliyor
     
     const char* mesg1 = "Your screen is too small.";
     const char* mesg2 = "(It should 75 COLS width and 16 LINES height minimum.)";
     
     int prevLINES = -1;
     int prevCOLS = -1;
     
          
     while (LINES < win_height || COLS < win_width){
          if (prevCOLS != COLS || prevLINES != LINES){ // work if resized.
               
               prevCOLS = COLS;
               prevLINES = LINES;
               
               int curr_y = (LINES-2)/2;
               
               clear();
               mvprintw(curr_y++,(COLS-strlen(mesg1))/2,"%s",mesg1);
               mvprintw(curr_y++,(COLS-strlen(mesg2))/2,"%s",mesg2);
               mvprintw(curr_y,(COLS-(strlen("Current Size : COLS-> ,LINES-> ")+findDigits(COLS)+findDigits(LINES)))/2,"Current Size : COLS-> %d, LINES-> %d",prevCOLS,prevLINES);
               refresh();
          }
          
          int ch = getch();
          if (ch == KEY_RESIZE) continue;
          
          // napms(50); //cpu'yu yormamak için kısa bekleme diyor ai
     } // work only if size is changed.
          
     // nodelay(stdscr, FALSE); ai ekledi ne olduğu hakkında hiçbir fikrim yok
     clear();
     refresh();
     
     alignWin(currwin);
     
     // ekranı geri yükle (sen bir dahisin)
     
}

void bold_box(WINDOW* win){ //wide character kullandığımız için artık -lncurses değil -lncursesw flagini kullanıcaz.
     // ayrıca #include <ncursesw/ncurses.h>
     // Define the wide characters for the border
     
     cchar_t ls, rs, ts, bs, tl, tr, bl, br;

     setcchar(&ls, L"┃", A_BOLD, 0, NULL); // Left side
     setcchar(&rs, L"┃", A_BOLD, 0, NULL); // Right side
     setcchar(&ts, L"━", A_BOLD, 0, NULL); // Top side
     setcchar(&bs, L"━", A_BOLD, 0, NULL); // Bottom side
     setcchar(&tl, L"┏", A_BOLD, 0, NULL); // Top left
     setcchar(&tr, L"┓", A_BOLD, 0, NULL); // Top right
     setcchar(&bl, L"┗", A_BOLD, 0, NULL); // Bottom left
     setcchar(&br, L"┛", A_BOLD, 0, NULL); // Bottom right

     // Apply the border to the window
     wattron(win, A_BOLD);
     wborder_set(win, &ls, &rs, &ts, &bs, &tl, &tr, &bl, &br);
     // wrefresh(win); // zaten redraw içinde wrefresh ediyoruz.
}
