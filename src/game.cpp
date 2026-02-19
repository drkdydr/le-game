#include "game.h"
#include <locale.h> //for setlocale
#include <cstdlib>
#include <ncurses.h>
#include <ncursesw/ncurses.h>
#include <string.h>
#include <algorithm>


void Game::initialize(){
     
     setlocale(LC_ALL, ""); //for unicode char display
     
     initscr();
     cbreak();
     noecho();
     curs_set(0); // makes cursor invisible
     
     nodelay(mainwin,true);
     
}

void Game::start(){
     
     // inMain = true; // initialized in game.h already
     
     mainwin = newwin(1,1,0,0);
     
     startbutt = new BUTTON(mainwin,"START",9); //index olduğu için bir eksiği
     exitbutt = new BUTTON(mainwin,"EXIT",12);
     
     game1butt = new BUTTON(mainwin,"SPACE SHOOTERS",6);
     game2butt = new BUTTON(mainwin,"SPACE SHOOTERS",9);
     game3butt = new BUTTON(mainwin,"SPACE SHOOTERS",12);
     
     windowTitle = "MAIN MENU";
     
     alignWin();
     
     while(true){
          
          
          int ch = getch(); // burada bir sorun var getch bizim input vermemiz için bekleyecek fakat biz bu beklemeyi istemiyoruz akıcı şekilde oynamak istiyoruz.
          if (ch == KEY_RESIZE){
               alignWin();
               continue;
          }
          // box(mainwin,0,0);
          // wrefresh(mainwin);
          // if (inMain){
          //      handleMain(ch);
          //      drawMain();
          // }else if (inSelect){
          //      handleSelec(ch);
          //      drawSelec();
          // }

     }
     
     endwin();
}

void Game::alignWin(){
     
     clear();
     refresh(); // bunlar alignWin'de while'ın bir satır üzerindeyken eski mainwin kalıntıları kalıyordu ekranda (araştır)
     //bunun sebebi büyük ihtimalle window size değiştiği için wclear eski kordinatlarla yeni yerleri silmeye çalışıyor ondan oluyor yani
     
     int h = std::min(LINES, win_height);
     int w = std::min(COLS, win_width);
     
     werase(mainwin);
     wresize(mainwin,h,w);
     mvwin(mainwin, (LINES-h)/2, (COLS-w)/2);
     werase(mainwin);
     
     if (COLS < win_width || LINES < win_height) 
          resizeNotif(h,w);
     else{
          box(mainwin,0,0);
          mvwprintw(mainwin,0,1," %s ", windowTitle); //alignwin sadece align yapsın box işlemi dışarıda kalsın istiyorum (resizeNotif dışarıda mı olsun içeride mi daha düşünmedim.)
     } 
     
     // box(mainwin,0,0);
     //      wprintw(mainwin, "C: %d, L: %d" , COLS,LINES);
     
     wrefresh(mainwin);
}

void Game::resizeNotif(int h, int w){ // acaba daha iyi nasıl yazabilirdim?
     //sorun şu an ekran küçülünce otomatik olarak uyarı moduna geçiyor fakat hem geri çıkmıyor hem de mod içinde güncellenmiyor.
     //difference detecting kısmında bir sıkıntı olabilir.
          
     // ekrandaki her şeyi sil
     //windowlar üst üste binebiliyor
     
     const char* mesg1 = "Your screen is too small.";
     const char* mesg2 = "(It should 75 COLS width and 16 LINES height minimum.)";
     
     int curr_y = (h-2)/2;
     
     mvwprintw(mainwin, curr_y++,(w-strlen(mesg1))/2,"%s",mesg1);
     mvwprintw(mainwin, curr_y++,(w-strlen(mesg2))/2,"%s",mesg2);
     mvwprintw(mainwin, curr_y,(w-(strlen("Current Size : COLS-> ,LINES-> ")+findDigits(w)+findDigits(h)))/2,"Current Size : COLS-> %d, LINES-> %d",w,h);
     
}

void Game::handleMain(int input){
     if (startbutt == nullptr){
     }
}

void Game::drawMain(){}


void Game::handleSelec(int input){
}

void Game::drawSelec(){}

int findDigits(int num){
     if (num < 0) return findDigits(-num);
     if (num == 0) return 0;
     return 1 + findDigits(num/10);
}



void bold_box(WINDOW* win){ //wide character kullandığımız için artık -lncurses değil -lncursesw flagini kullanıcaz.
     // ayrıca #include <ncursesw/ncurses.h>
     // Define the wide characters for the border
     
     cchar_t ls, rs, ts, bs, tl, tr, bl, br;

     setcchar(&ls, L"┃", A_ATTRIBUTES, 0, NULL); // Left side
     setcchar(&rs, L"┃", A_ATTRIBUTES, 0, NULL); // Right side
     setcchar(&ts, L"━", A_ATTRIBUTES, 0, NULL); // Top side
     setcchar(&bs, L"━", A_ATTRIBUTES, 0, NULL); // Bottom side
     setcchar(&tl, L"┏", A_ATTRIBUTES, 0, NULL); // Top left
     setcchar(&tr, L"┓", A_ATTRIBUTES, 0, NULL); // Top right
     setcchar(&bl, L"┗", A_ATTRIBUTES, 0, NULL); // Bottom left
     setcchar(&br, L"┛", A_ATTRIBUTES, 0, NULL); // Bottom right

     // Apply the border to the window
     wborder_set(win, &ls, &rs, &ts, &bs, &tl, &tr, &bl, &br);
     // wrefresh(win); // zaten redraw içinde wrefresh ediyoruz.
}
