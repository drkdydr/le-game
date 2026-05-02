#include "legame.h"
#include <locale.h> //for setlocale
#include <cstdlib>
#include <ncurses.h>
#include <ncursesw/ncurses.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <unistd.h> // for usleep();
#include <vector>
#include <chrono>

//todo :
// warninglerden kurtul. (BUTTON'da char* variable'ın başına const yazdım oldu.)

//critical:
//comingsoon'da esc yapınca pause ekranında resume ve restartbutt'lar aynı anda yanıyor. düzelince hep düzeliyor.

// optional
// main menu falan sadece ekran boyutu falan değiştiği zaman değişsin
// butonlar sadece switch sırasında tekrar yazılsın istiyorum.

// bool LeGame::inMenus = true;
bool LeGame::inMain = true; //sınıf içindeki static data member'lar cpp dosyasında "dışarıda" tanımlanır.
bool LeGame::inSelect = false;
bool LeGame::inPause = false;
// bool LeGame::inGames = false;
bool LeGame::inGame1 = false;
bool LeGame::inGame2 = false;
bool LeGame::inGame3 = false;
bool LeGame::inVictory = false;
bool LeGame::inGameOver = false;

void LeGame::initialize(){
     
     setlocale(LC_ALL, ""); //for unicode char display (bold box characters)
     
     initscr();
     cbreak();
     noecho();
     curs_set(0); // makes cursor invisible
     
     windowTitle = "MAIN MENU";
     
     mainwin = newwin(0,0,0,0);
     
     game1 = new SpaceShooters(mainwin);
     // game2 = new DinoGame();
     game2 = new ComingSoon();
     game3 = new ComingSoon();
     
     startbutt = new BUTTON(mainwin,"START",11); //index olduğu için bir eksiği
     exitbutt = new BUTTON(mainwin,"EXIT",14);
     
     game1butt = new BUTTON(mainwin,game1->getName(),8);
     // game2butt = new BUTTON(mainwin,"DINO GAME",11);
     game2butt =  new BUTTON(mainwin,game2->getName(),11);
     game3butt = new BUTTON(mainwin,game3->getName(),14);
     
     resumebutt = new BUTTON(mainwin,"RESUME",7);
     restartbutt = new BUTTON(mainwin,"RESTART",10);
     quitbutt = new BUTTON(mainwin,"QUIT",13);
     
     allButtons = {startbutt, exitbutt, game1butt, game2butt, game3butt, resumebutt, quitbutt};
     mainButtons = {startbutt, exitbutt};
     selecButtons = {game1butt, game2butt, game3butt};
     pauseButtons = {resumebutt, restartbutt, quitbutt};
     victoryButtons = {restartbutt, quitbutt};
     gameOverButtons = {restartbutt, quitbutt};
     
     alignWin(); // bunu yapmadan button oluşturunca sıkıntı oluyor.
     
     startbutt->isSelected = true;
     game1butt->isSelected = true;
     resumebutt->isSelected = true;
     
     // nodelay(stdscr,true);
     nodelay(mainwin,true);
     
     // Keypad:
     // If enabled (bf is TRUE) then when an input character reading function reads ESC, it waits for further input corresponding to an escape sequence defined by the terminal type description.
     // If disabled (bf is FALSE), curses does not treat function keys specially and the program has to interpret escape sequences itself.

     keypad(mainwin,false);
}

void LeGame::start(){

      initialize();
     
     inMain = true;
     // inMenus = true;
     
     while(true){
          
          if (exitWanted) break;
          
          // flushinp();
          in = wgetch(mainwin);

          // if (in == ERR && !inGames) continue; // waits until input comes (if not in games)
          //this does not work but I will find a way to refresh window only when necessary
          
          if (in == KEY_RESIZE) alignWin();
          
          if (in == '\033') // ALT and ESC has same ascii code
               handleEscape();
          
          if (COLS < win_width || LINES < win_height){
                if (inGame1 || inGame2 || inGame3) inPause = true;
               resizeNotif();
               continue;
          }
     
          wclear(mainwin);
          box(mainwin,0,0);
          mvwprintw(mainwin,0,1," %s ", windowTitle);

          if (inMain){
               handleMain(in);

          }else if (inSelect) {
              handleSelec(in);

          }else if (inPause) {
              handlePause(in); 

          }else if (inVictory){
               handleVictory(in);

          }else if (inGameOver){
               handleGameOver(in);

          }else if (inGame1){
               game1->start(in);

          } else if (inGame2){
               game2->process(mainwin,in);
               
          } else if (inGame3){
               game3->process(mainwin,in); 
          }
         
          doupdate();
          usleep(20000); // sleep for x microseconds (10^-6) (20 milliseconds = 2 salise)
     }
     
     endwin();
}

void LeGame::handleEscape(){
     int in2 = wgetch(mainwin);
     if (in2 == '['){
          int in3 = wgetch(mainwin);
          switch(in3){
               case 'A': // ESC(27) + [(91) + A(65) -> UP ARROW
                    in = 'w';
               break;
               case 'B': // ESC(27) + [(91) + B(66) -> DOWN ARROW
                    in = 's'; 
               break;
               case 'C': // ESC(27) + [(91) + C(67) -> RIGHT ARROW
                    in = 'd'; 
               break;
               case 'D': // ESC(27) + [(91) + D(68) -> LEFT ARROW
                    in = 'a';
               break;
               default:
               ungetch(in2);
               ungetch(in3);
               break;
          }
     }else
          ungetch(in2);
}

void LeGame::alignWin(){
     
     int h = std::min(LINES, win_height);
     int w = std::min(COLS, win_width);
     
     werase(mainwin);
     wnoutrefresh(mainwin);
     
     wresize(mainwin,h,w);
     mvwin(mainwin, (LINES-h)/2, (COLS-w)/2);
     
     if (inMain)
          for(BUTTON* b : mainButtons) alignButton(b);
     else if (inSelect)
          for(BUTTON* b : selecButtons) alignButton(b);
     else if (inPause)
          for(BUTTON* b : pauseButtons) alignButton(b);
     else if (inVictory)
          for(BUTTON* b : victoryButtons) alignButton(b);
     else if (inGameOver)
          for(BUTTON* b : gameOverButtons) alignButton(b);
     
     touchwin(mainwin);
     wnoutrefresh(mainwin);
     
}

void LeGame::hideButton(BUTTON* b){
     if (b == nullptr) return;
     
     if (b->win != nullptr){ 
          //subwinleri move yapmak seg faultlara veya bende olduğu gibi stdscr'de 0,0'a gitmelerine sebep olabiliyormuş.
          delwin(b->win);
          b->win = nullptr;
     }
     
     b->isHidden = true;
     
     // wnoutrefresh(b->win); // dikkat et! sıkıntı çıkarabilir
     
}

void LeGame::alignButton(BUTTON* b){
     
     if (b == nullptr) return;
     
     if (b->win != nullptr) {
          delwin(b->win);
          b->win = nullptr;
     }
     
     int main_w = getmaxx(mainwin);
     
     b->win = derwin(mainwin, b->default_height, b->default_width,b->rel_y, (main_w - b->default_width)/2 ); // derwin is subwin but relative to parent win
     
     if (b->win != nullptr){ // if derwin throw ERR
          nodelay(b->win, true);
          werase(b->win);
          wnoutrefresh(b->win);
     }
     
     b->isHidden = false;
     
}

void LeGame::resizeNotif(){ // acaba daha iyi nasıl yazabilirdim?
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

void LeGame::handleMain(int input){
     const static int button_count = 2;
     static int curr_idx = 0;

      if (!startbutt->win || !exitbutt->win){
            windowTitle = mainName;
            startbutt->isSelected = true;
            exitbutt->isSelected = false;

            curr_idx = 0;

            for (BUTTON* b : mainButtons) alignButton(b);
      }

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
                    for(BUTTON* b : mainButtons) hideButton(b);
               }
               else if (exitbutt->isSelected){
                    exitWanted = true;
                    return;
               }
               break;
          case 'q' :case 27:
               exitWanted = true;
               return;
          default:
               break;
     }
     drawMain();
}

void LeGame::drawMain(){
     printLogo(game_logo,2);
     // printVer();
     wnoutrefresh(mainwin);
     exitbutt->drawButt();
     startbutt->drawButt();
}

// void Game::printVer(){
//      const char* text = "VERSION ";
//      mvwprintw(mainwin,win_height-2,win_width - (strlen(text) + strlen(version) + 2),"%s%s",text,version);
// }

void LeGame::printLogo(std::vector<const char*> &logo, int y_idx){
     
     int w_height, w_width;
     
     getmaxyx(mainwin, w_height, w_width);
     
     for(const char* line : logo)
          mvwprintw(mainwin,y_idx++,(w_width - strlen(line))/2,"%s",line);
     
}

void LeGame::handleSelec(int input){
     
     const static int button_count = 3;
     static int curr_idx = 0;

     if (!game1butt->win || !game2butt->win || !game3butt->win){ 
        windowTitle = selecName;
        game1butt->isSelected = true;
        game2butt->isSelected = false;
        game3butt->isSelected = false;

        curr_idx = 0;

        for (BUTTON* b : selecButtons) alignButton(b);
     }
     
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
               
               if (game1butt->isSelected){
                     windowTitle = game1->getName();
                    inGame1 = true;
                    game1->reset(); // 3 satır set şu time set bu time yapacağıma bunu koymayı daha temiz gördüm
               }

               if (game2butt->isSelected){
                     windowTitle = game2->getName();
                    inGame2 = true;
               }

               if (game3butt->isSelected){
                     windowTitle = game3->getName();
                    inGame3 = true;
               }

              inSelect = false;
               // inGames = true;
               for(BUTTON* b : selecButtons) hideButton(b);

          break;
          case 'q' : case 27:
               inSelect = false;
               inMain = true;
               for(BUTTON* b : selecButtons) hideButton(b);
               
                break;
          default:
          break;
     }
     drawSelec();
}

void LeGame::drawSelec(){
     printLogo(select_logo,3);
     wnoutrefresh(mainwin);
     game1butt->drawButt();
     game2butt->drawButt();
     game3butt->drawButt();
}

void LeGame::handlePause(int input){
     
     const static int button_count = 3;
     static int curr_idx = 0;

     if (!resumebutt->win || !restartbutt->win || !quitbutt->win){ // switching pause new
        windowTitle = pauseName;
        resumebutt->isSelected = true;
        restartbutt->isSelected = false;
        quitbutt ->isSelected = false;

        curr_idx = 0;

        for (BUTTON* b : pauseButtons) alignButton(b);
     }
     
     switch(input){
          case KEY_DOWN : case 's' : case 'j':
               pauseButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + 1) % button_count;
               pauseButtons[curr_idx]->isSelected = true;
               break;
          case KEY_UP : case 'w' : case 'k' :
               pauseButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + button_count - 1) % button_count;
               pauseButtons[curr_idx]->isSelected = true;
               break;
          case KEY_ENTER : case '\n' : case ' ':
               if (!quitbutt->isSelected){
                    
                    if (inGame1){
                        windowTitle = game1->getName();
                          game1->setEnterPoint();
                    }
                    
                    if (inGame2)
                        windowTitle = game2->getName();
                    
                    if (inGame3)
                        windowTitle = game3->getName();
                    
                    if(restartbutt->isSelected){
                         if(inGame1)
                              game1->reset();
                         else if (inGame2)
                              game2->reset();
                         else if (inGame3)
                              game3->reset();
                    }
                    
               }else{
                    
                    if(inGame1)
                         game1->reset();
                    else if (inGame2)
                         game2->reset();
                    else if (inGame3)
                         game3->reset();
                    
                   inGame1 = inGame2 = inGame3 = false; 
                   // inGames = false;
                   inSelect = true;
                   
               }

               for(BUTTON* b : pauseButtons) hideButton(b);
               inPause = false;
               break;
               
          case 'q': case 27:
          
               if(inGame1)
                    game1->reset();
               else if (inGame2)
                    game2->reset();
               else if (inGame3)
                    game3->reset();
                    
               inGame1 = inGame2 = inGame3 = false; 
               inSelect = true;
               
               inPause = false;

               for(BUTTON* b : pauseButtons) hideButton(b);
               
               break;
               
          default:
               break;
     }
     drawPause();
}

void LeGame::drawPause(){
     
     if(inGame1){
          game1->print();
     }else if (inGame2){
          game2->print(mainwin);
     }else if (inGame3){
          game3->print(mainwin);
     }
     wnoutrefresh(mainwin);
     printLogo(pause_logo,2);
     wnoutrefresh(mainwin);
     
     resumebutt->drawButt();
     restartbutt->drawButt();
     quitbutt->drawButt();
}

void LeGame::drawScore(){ // implemented only for game1 if you wanna add new games handle it
     const char* text = "SCORE:";
     mvwprintw(mainwin, 8, (win_width - strlen(text))/2, "%s", text);
     if (inGame1){
           if (inVictory)
                 mvwprintw(mainwin, 9, (win_width - findDigits(game1->getScore() + game1->timeBonus()))/2, "%d", game1->getScore() + game1->timeBonus());
           if (inGameOver)
                 mvwprintw(mainwin, 9, (win_width - findDigits(game1->getScore()))/2, "%d", game1->getScore());
     }

     
}

void LeGame::handleVictory(int input){
     
     const static int button_count = 2;
     static int curr_idx = 0;

     if (!restartbutt->win || !quitbutt->win){   
         windowTitle = victoryName; //process functionlarını bunu kaçınılmaz kılacak şekilde yazmışız da o yüzden
         restartbutt->isSelected = true;
         quitbutt ->isSelected = false;

         curr_idx = 0;

         for (BUTTON* b : victoryButtons) alignButton(b);
     }
     
     switch(input){
          case KEY_DOWN : case 's' : case 'j':
               victoryButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + 1) % button_count;
               victoryButtons[curr_idx]->isSelected = true;
               break;
          case KEY_UP : case 'w' : case 'k' :
               victoryButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + button_count - 1) % button_count;
               victoryButtons[curr_idx]->isSelected = true;
               break;
          case KEY_ENTER : case '\n' : case ' ':
               
               if (inGame1)
                    game1->reset();
               if (inGame2)
                    game2->reset();
               if (inGame3)
                    game3->reset();
               
               if (quitbutt->isSelected) {
                    
                    inGame1 = inGame2 = inGame3 = false; 
                    inSelect = true;
                    
                    for(BUTTON* b : victoryButtons) hideButton(b);
                    
               }else{
                    
                    if (inGame1)
                        windowTitle = game1->getName();
                    
                    if (inGame2)
                        windowTitle = game2->getName();
                    
                    if (inGame3)
                        windowTitle = game3->getName();
                    
               }
               
               inVictory = false;
               break;
         
          case 'q': case 27:
          
               if (inGame1)
                    game1->reset();
               if (inGame2)
                    game2->reset();
               if (inGame3)
                    game3->reset();
               
               inGame1 = inGame2 = inGame3 = false; 
               inSelect = true;

               for(BUTTON* b : victoryButtons) hideButton(b);
               
               inVictory = false;
               break;
               
          default:
               break;
     }
     drawVictory();
}

void LeGame::drawVictory(){

     if(inGame1){
          game1->print();
     }else if (inGame2){
          game2->print(mainwin);
     }else if (inGame3){
          game3->print(mainwin);
     }

     wnoutrefresh(mainwin);
     printLogo(victory_logo,3);
     drawScore();
     wnoutrefresh(mainwin);
     
     restartbutt->drawButt();
     quitbutt->drawButt();
}

void LeGame::handleGameOver(int input){
     windowTitle = gameoverName; //process functionlarını bunu kaçınılmaz kılacak şekilde yazmışız da o yüzden
     
     const static int button_count = 2;
     static int curr_idx = 0;

     if (!restartbutt->win || !quitbutt->win){   
         windowTitle = gameoverName;
         restartbutt->isSelected = true;
         quitbutt ->isSelected = false;

         curr_idx = 0;

         for (BUTTON* b : gameOverButtons) alignButton(b);
     }
     
     switch(input){
          case KEY_DOWN : case 's' : case 'j':
               gameOverButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + 1) % button_count;
               gameOverButtons[curr_idx]->isSelected = true;
               break;
          case KEY_UP : case 'w' : case 'k' :
               gameOverButtons[curr_idx]->isSelected = false;
               curr_idx = (curr_idx + button_count - 1) % button_count;
               gameOverButtons[curr_idx]->isSelected = true;
               break;
          case KEY_ENTER : case '\n' : case ' ':
               
               if (inGame1)
                    game1->reset();
               if (inGame2)
                    game2->reset();
               if (inGame3)
                    game3->reset();
          
               if (quitbutt->isSelected) {
                    
                    inGame1 = inGame2 = inGame3 = false; 
                    inSelect = true;
                    
                    for(BUTTON* b : gameOverButtons) hideButton(b);
                    
               }else{
                    
                    if (inGame1)
                        windowTitle = game1->getName();
                    
                    if (inGame2)
                        windowTitle = game2->getName();
                    
                    if (inGame3)
                        windowTitle = game3->getName();
                    
               }
               inGameOver = false;
               break;
         
          case 'q': case 27:
          
               if (inGame1)
                    game1->reset();
               if (inGame2)
                    game2->reset();
               if (inGame3)
                    game3->reset();
          
               inGame1 = inGame2 = inGame3 = false; 
               inSelect = true;
               
               for(BUTTON* b : gameOverButtons) hideButton(b);
               
               inGameOver = false;
               break;
               
          default:
               break;
     }
     drawGameOver();
     
}

void LeGame::drawGameOver(){
     if(inGame1){
          game1->print();
     }else if (inGame2){
          game2->print(mainwin);
     }else if (inGame3){
          game3->print(mainwin);
     }
     wnoutrefresh(mainwin);
     printLogo(gameover_logo,3);
     drawScore();
     wnoutrefresh(mainwin);
     
     restartbutt->drawButt();
     quitbutt->drawButt();
}


int findDigits(int num){
     if (num < 0) return 1 + findDigits(-num); // eksi de bir basamak (matematiksel olmasa da burada böyle)
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
     // wnoutrefresh(win); // zaten redraw içinde wnoutrefresh ediyoruz.
}

BUTTON::BUTTON(WINDOW* parent, const char* cont, int rel_y_){
     default_width = 31;
     default_height = 3;
     rel_y = rel_y_;
     
     isSelected = false;
     isHidden = true;
     
     content = cont;

     win = nullptr;
     
     nodelay(win,true);
}

void BUTTON::drawButt(){
     
     if(isHidden || win == nullptr) return;
     
     werase(win);
     wnoutrefresh(win);
     
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
     wnoutrefresh(this->win);
     
}
