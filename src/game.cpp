#include "game.h"
#include <locale.h> //for setlocale
#include <cstdlib>
#include <ncurses.h>
#include <ncursesw/ncurses.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <unistd.h> // for usleep();
#include <vector>

//todo :
// warninglerden kurtul.
// space shooters class'ı oluştur ve oyunu orada yaz.
// main menu falan sadece ekran boyutu falan değiştiği zaman değişsin
// butonlarda dalgalanma oluyor.
// butonlar sadece switch sırasında tekrar yazılsın istiyorum.

//fikri unutmamak için yazıyorum:
// main ve select menuler 

void Game::initialize(){
     
     setlocale(LC_ALL, ""); //for unicode char display
     
     initscr();
     cbreak();
     noecho();
     curs_set(0); // makes cursor invisible
     
     windowTitle = "MAIN MENU";
     
     mainwin = newwin(0,0,0,0);
     
     alignWin(); // bunu yapmadan button oluşturunca sıkıntı oluyor.
     
     startbutt = new BUTTON(mainwin,"START",11); //index olduğu için bir eksiği
     exitbutt = new BUTTON(mainwin,"EXIT",14);
     
     game1butt = new BUTTON(mainwin,"SPACE SHOOTERS",8);
     game2butt = new BUTTON(mainwin,"DINO GAME",11);
     game3butt = new BUTTON(mainwin,"COMING SOON",14);
     
     resumebutt = new BUTTON(mainwin,"RESUME",9);
     quitbutt = new BUTTON(mainwin,"QUIT",12);
     
     allButtons = {startbutt, exitbutt, game1butt, game2butt, game3butt, resumebutt, quitbutt};
     mainButtons = {startbutt, exitbutt};
     selecButtons = {game1butt, game2butt, game3butt};
     pauseButtons = {resumebutt, quitbutt};
     
     // alignWin(); // bunu yapmadan button oluşturunca sıkıntı oluyor.
     
     startbutt->isSelected = true;
     game1butt->isSelected = true;
     resumebutt->isSelected = true;
     
     game1 = new SpaceShooters();
     game2 = new DinoGame();
     game3 = new ComingSoon();
     
     // nodelay(stdscr,true);
     nodelay(mainwin,true);
     keypad(mainwin,true);
     
}

void Game::start(){
     
     // inMain = true; // initialized in game.h already
     
     long tick = 0;
     
     while(true){
          
          if (exitWanted) break;
          
          int ch = wgetch(mainwin);
          
          if (ch == KEY_RESIZE) alignWin();
          
          if (COLS < win_width || LINES < win_height){
               // for(BUTTON* b : allButtons) hideButton(b);
               resizeNotif();
               continue;
          }
          
          if (ch == 27){
               int ch2 = wgetch(mainwin);
               if(ch2 == -1) ch = 'i'; // check for ALT or ESC
               // if ALT set key to unfunctional key
          }
     
          wclear(mainwin);
          box(mainwin,0,0);
          mvwprintw(mainwin,0,1," %s ", windowTitle);
          
          if (inMain){ 
               handleMain(ch);
          } else if (inSelect){
              handleSelec(ch);
          } else if (inPause){
              handlePause(ch); 
          } else if (inGame1){
               inPause = game1->process(mainwin,ch);
          } else if (inGame2){
               inPause = game2->process(mainwin,ch);
          } else if (inGame3){
               inPause = game3->process(mainwin,ch); 
               //usr/include/c++/15/bits/stl_vector.h:1263: std::vector<_Tp, _Alloc>::reference std::vector<_Tp, _Alloc>::operator[](size_type) [with _Tp = const char*; _Alloc = std::allocator<const char*>; reference = const char*&; size_type = long unsigned int]: Assertion '__n < this->size()' failed
          }
          
          usleep(10000);
          tick++;
     }
     
     endwin();
}

void Game::alignWin(){
     
     int h = std::min(LINES, win_height);
     int w = std::min(COLS, win_width);
     
     werase(mainwin);
     wrefresh(mainwin);
     
     wresize(mainwin,h,w);
     mvwin(mainwin, (LINES-h)/2, (COLS-w)/2);
     
     if (inMain)
          for(BUTTON* b : mainButtons) alignButton(b);
     else if (inSelect)
          for(BUTTON* b : selecButtons) alignButton(b);
     else if (inPause)
          for(BUTTON* b : pauseButtons) alignButton(b);
     
     touchwin(mainwin);
     wrefresh(mainwin);
     
}

void Game::hideButton(BUTTON* b){
     if (b == nullptr) return;
     
     if (b->win != nullptr){ //subwinleri move yapmak seg faultlara veya bende olduğu gibi stdscr'de 0,0'a gitmelerine sebep olabiliyormuş.
          delwin(b->win);
          b->win = nullptr;
     }
     
     b->isHidden = true;
     
     // wrefresh(b->win); // dikkat et! sıkıntı çıkarabilir
     
}

void Game::alignButton(BUTTON* b){
     
     if (b == nullptr) return;
     
     if (b->win != nullptr) {
          delwin(b->win);
          b->win = nullptr;
     }
     
     int main_w = getmaxx(mainwin);
     
     b->win = derwin(mainwin, b->default_height, b->default_width,b->rel_y, (main_w - b->default_width)/2 );
     
     if (b->win != nullptr){ // if derwin throw ERR
          nodelay(b->win, true);
          werase(b->win);
          wrefresh(b->win);
     }
     
     b->isHidden = false;
     
}

void Game::resizeNotif(){ // acaba daha iyi nasıl yazabilirdim?
     //windowlar üst üste binebiliyor
     
     int height, width;
     
     getmaxyx(mainwin, height, width);
     
     const char* mesg1 = "Your screen is too small.";
     std::string mesg2 = "(It should " + std::to_string(win_width) + " COLS width and "+ std::to_string(win_height) + " LINES height minimum.)";
     
     int curr_y = (height-2)/2;
     
     // werase(mainwin);
     mvwprintw(mainwin, curr_y++,(width-strlen(mesg1))/2,"%s",mesg1);
     mvwprintw(mainwin, curr_y++,(width-(mesg2.size()))/2,"%s",mesg2.c_str());
     mvwprintw(mainwin, curr_y,(width-(strlen("Current Size : COLS-> ,LINES-> ")+findDigits(width)+findDigits(height)))/2,"Current Size : COLS-> %d, LINES-> %d",width,height);
     wrefresh(mainwin);
}

void Game::handleMain(int input){
     const static int button_count = 2;
     static int curr_idx = 0;
     switch(input){
          case KEY_UP : case 'w' : case 'k' : case KEY_DOWN : case 's' : case 'j': // aga ikisi de aynı şeye varıyor.
               mainButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + 1) % button_count;
               mainButtons[curr_idx]->isSelected = true;
               break;
          case KEY_ENTER : case ' ': case '\n': 
          // KEY_ENTER numpad'deki enter tuşunu
          // \n ise klavyedeki enter'ı temsil ediyormuş.
               if (startbutt->isSelected){
                    inMain = false;
                    inSelect = true;
                    windowTitle = "SELECTION MENU";
                    for(BUTTON* b : mainButtons) hideButton(b);
                    for(BUTTON* b : selecButtons) alignButton(b);
               }
               else if (exitbutt->isSelected){
                    exitWanted = true;
                    return;
               }
               curr_idx = 0;
               startbutt->isSelected = true;
               quitbutt->isSelected = false; // redundant but I wanted to keep.
               break;
          case 'q' :
               exitWanted = true;
               return;
          default:
               break;
     }
     drawMain();
}

void Game::drawMain(){
     printLogo(game_logo,2);
     wrefresh(mainwin);
     exitbutt->drawButt();
     startbutt->drawButt();
}

void Game::printLogo(std::vector<const char*> &logo, int y_idx){
     
     int w_height, w_width;
     
     getmaxyx(mainwin, w_height, w_width);
     
     for(const char* line : logo)
          mvwprintw(mainwin,y_idx++,(w_width - strlen(line))/2,"%s",line);
     
}

void Game::handleSelec(int input){
     
     const static int button_count = 3;
     static int curr_idx = 0;
     
     switch(input){
          case KEY_UP : case 'w' : case 'k' :
               selecButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + button_count - 1) % button_count;
               selecButtons[curr_idx]->isSelected = true;
          break; 
          case KEY_DOWN : case 's' : case 'j':
               selecButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + 1) % button_count;
               selecButtons[curr_idx]->isSelected = true;
          break;
          case KEY_ENTER : case ' ': case '\n': 
          // KEY_ENTER numpad'deki enter tuşunu
          // \n ise klavyedeki enter'ı temsil ediyormuş.
               BUTTON* selected;
               if (game1butt->isSelected){
                    game1butt->isSelected = false;
                    selected = game1butt;
                    inSelect = false;
                    inGame1 = true;
               }
               if (game2butt->isSelected){
                    game2butt->isSelected = false;
                    selected = game2butt;
                    inSelect = false;
                    inGame2 = true;
               }
               if (game3butt->isSelected){
                    game3butt->isSelected = false;
                    selected = game3butt;
                    inSelect = false;
                    inGame3 = true;
               }
               windowTitle = selected->content.c_str();
               game1butt->isSelected = true;
               curr_idx = 0;
          break;
          case 'q' :
               curr_idx = 0;
               game1butt->isSelected = true;
               game2butt->isSelected = false;
               game3butt->isSelected = false;
               
               inSelect = false;
               inMain = true;
               windowTitle = "MAIN MENU";
               for(BUTTON* b : selecButtons) hideButton(b);
               for(BUTTON* b : mainButtons) alignButton(b);
               
          break;
          default:
          break;
     }
     drawSelec();
}

void Game::drawSelec(){
     printLogo(select_logo,3);
     wrefresh(mainwin);
     game1butt->drawButt();
     game2butt->drawButt();
     game3butt->drawButt();
}

void Game::handlePause(int input){
     const static int button_count = 2;
     static int curr_idx = 0;
     switch(input){
          case KEY_UP : case 'w' : case 'k' :
               pauseButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + 1) % button_count;
               pauseButtons[curr_idx]->isSelected = true;
               break;
          case KEY_DOWN : case 's' : case 'j':
               pauseButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + button_count - 1) % button_count;
               pauseButtons[curr_idx]->isSelected = true;
               break;
          case KEY_ENTER : case '\n' : case ' ':
               if (resumebutt->isSelected){
                    inPause = false;
               }
               if (quitbutt->isSelected){
                    
                    if(inGame1){
                         game1->reset();
                    }else if (inGame2){
                         game2->reset();
                    }else if (inGame3){
                         game3->reset();
                    }
                    
                    inGame1 = inGame2 = inGame3 = false;
                    inSelect = true;
                    windowTitle = "SELECTION MENU";
               }
               curr_idx = 0;
               resumebutt->isSelected = true; // çıkarken bulduğumuz gibi bırakalım.
               quitbutt->isSelected = false;
               break;
          default:
               break;
     }
}
void Game::drawPause(){
     printLogo(pause_logo,4);
     wrefresh(mainwin);
     
     if(inGame1){
          game1->print(mainwin);
     }else if (inGame2){
          game2->print(mainwin);
     }else if (inGame3){
          game3->print(mainwin);
     }
     wrefresh(mainwin);
     
     resumebutt->drawButt();
     quitbutt->drawButt();
}

int findDigits(int num){
     if (num < 0) return findDigits(-num);
     if (num == 0) return 0;
     return 1 + findDigits(num/10);
}



void bold_box(WINDOW* win){ 
     // wide character kullandığımız için artık -lncurses değil -lncursesw flagini kullanıcaz.
     // ayrıca #include <ncursesw/ncurses.h>
     // Define the wide characters for the border
     
     cchar_t ls, rs, ts, bs, tl, tr, bl, br;

     setcchar(&ls, L"┃", A_NORMAL, 0, NULL); // Left side
     setcchar(&rs, L"┃", A_NORMAL, 0, NULL); // Right side
     setcchar(&ts, L"━", A_NORMAL, 0, NULL); // Top side
     setcchar(&bs, L"━", A_NORMAL, 0, NULL); // Bottom side
     setcchar(&tl, L"┏", A_NORMAL, 0, NULL); // Top left
     setcchar(&tr, L"┓", A_NORMAL, 0, NULL); // Top right
     setcchar(&bl, L"┗", A_NORMAL, 0, NULL); // Bottom left
     setcchar(&br, L"┛", A_NORMAL, 0, NULL); // Bottom right

     // Apply the border to the window
     wborder_set(win, &ls, &rs, &ts, &bs, &tl, &tr, &bl, &br);
     // wrefresh(win); // zaten redraw içinde wrefresh ediyoruz.
}

BUTTON::BUTTON(WINDOW* parent, char* cont, int rel_y_){
     default_width = 31;
     default_height = 3;
     rel_y = rel_y_;
     
     isSelected = false;
     isHidden = true;
     
     content = cont;
     
     // win = derwin(parent, 0, 0, 0, 0); // mainwin size independant
     // newwin(0,0,0,0) tam ekran window oluşturuyormuş.
     // win = derwin(parent,0,0,0,0);
     win = nullptr;
     
     nodelay(win,true);
}

void BUTTON::drawButt(){
     
     if(isHidden || win == nullptr) return;
     
     werase(win);
     wrefresh(win);
     
     if (isSelected){
          wattron(win, A_BOLD);
          bold_box(win);
          wattroff(win,A_BOLD);
     }else{
          box(win,0,0);
     }
     
     int text_y = default_height / 2;
     int text_x = (default_width - static_cast<int>(content.size())) / 2;
     
     mvwprintw(win,text_y,text_x,"%s",content.c_str());
     wrefresh(this->win);
     
}