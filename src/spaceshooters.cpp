#include "spaceshooters.h"
#include "legame.h"
#include <cstdlib>
#include <ncurses.h>
#include <algorithm>
#include <unistd.h>

// todo:

// stop timer while pause
// timer records time in selection menu;
// timer also records time from the start of program because I chronometer starts when ss object initialized

// pause'a falan geçerken şu ana kadar geçen süreyi bir değişkene kaydet.
// her inGame1 setlendiğinde bir enterPoint tut ve her çıkış yapıldığında bir exitPoint tut eğer exitPoint enter point'ten önce ise arasındaki farkı startPoint'e kaydır.

// immortal mode [optional]
// die animation (enemy/player) [optional]

const int Enemy::killscore = 200;
const int SuperEnemy::killscore = 500;

SpaceShooters::SpaceShooters(WINDOW* &win_){
     
     tick = 0;
     gameFinished = false;
     win = win_;
     player = new Player(win,14,34);
     int idx = 0;
     Enemy* e;
     SuperEnemy* se;
     
     for(int x = 3; x<=67; x += 8){
          se = new SuperEnemy(win,3,x);
          superEnemies.push_back(se);
     }
     for(int x = 7; x<=63; x += 8){
          e = new Enemy(win,5,x);    
          enemies.push_back(e);
     }
     for(int x = 3; x<=67; x += 8){
          e = new Enemy(win,7,x);    
          enemies.push_back(e);
     }
     
      // startPoint = std::chrono::steady_clock::now();
      // exitPoint = std::chrono::steady_clock::now();

}

void SpaceShooters::start(int input){ //r1 r2 olayından çok memnun değilim ama iyi de duruyorlar.
     
     if (tick >= 100) tick = 0; //prevent overflow
     tick++;
     // if (tick % 50 != 0) return false; //tick rate for game (will be set more precisely)
     //tick'i buraya eklemek mallık 50 tane çalışmadan birinde çalışıyor.
     
     // flushinp(); // bunun oynayışını daha çok sevdim.
     // usleep(150000);
     
      if (exitPoint < enterPoint){
            auto timeinMenus = enterPoint - exitPoint;
            startPoint += timeinMenus;
            exitPoint = enterPoint; // tekrar tekrar eklemesin diye
      }

     for(Enemy* e : enemies){
          if(tick%10 == 0){
              EnemyBullet* eb = e->shoot();
              if (eb != nullptr)
                   enemyBullets.push_back(eb);
          } 
     }

     for(SuperEnemy* se: superEnemies){
           if (tick%12 == 0){
                 EnemyBullet* eb = se->shoot();
                 if (eb != nullptr)
                       enemyBullets.push_back(eb);
           }
     }
     
     for(EnemyBullet* eb : enemyBullets){
          if (tick%eb->getSpeed() == 0)
          if(eb->move()) enemyBullets.erase(find(enemyBullets.begin(),enemyBullets.end(),eb)); // delete bullet if exceed border.
                                                                                               //
          if (player->doesHit(eb)){
               lives--;
               enemyBullets.erase(find(enemyBullets.begin(), enemyBullets.end(), eb));
          }
     }
     
     for(PlayerBullet* pb : playerBullets){

          if(tick%pb->getSpeed() == 0)
          if(pb->move()) playerBullets.erase(find(playerBullets.begin(),playerBullets.end(),pb));

          for(Enemy* e : enemies){
                  if(e->doesHit(pb)){
                    score += e->killscore;
                    enemyremains--;
                    playerBullets.erase(find(playerBullets.begin(), playerBullets.end(), pb));
                    break;
                  }
          }

          for(SuperEnemy* se : superEnemies){
                if (se->doesHit(pb)){
                      if (se->getIsDead()){
                            score += se->killscore;
                            enemyremains--;
                      }
                      playerBullets.erase(find(playerBullets.begin(), playerBullets.end(), pb));
                      break;
                }
          }
     }
     
     if (lives <= 0) { //gameover
         LeGame::inGameOver = true;
         gameFinished = true;
         return;
     }

     if (enemyremains <= 0) { //victory
         LeGame::inVictory = true;
         gameFinished = true;
         return;
     }

     switch(input){
          case 27 : case 'q':
               LeGame::inPause = true;
               exitPoint = std::chrono::steady_clock::now();
               break;
          case 'h' : case 'a' : case KEY_LEFT :
               player->move(LEFT);
               break;
          case 'l' : case 'd' : case KEY_RIGHT :
               player->move(RIGHT);
               break;
          case ' ' : case 'x': {
               now = std::chrono::steady_clock::now();
               auto sinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPlayerShot); // burada bir koşula göre yeni bir değişken oluşturduğumuz için scope içine almalıyız
               if (sinceLastShot.count() >= 700){
                     playerBullets.push_back(player->shoot());
                     lastPlayerShot = std::chrono::steady_clock::now();
               }                     
          }
                  break;
          default :
               break;
     }


     print();
}

void SpaceShooters::printLives(){
     mvwprintw(win,win_height-3,2,"LIVES: %d",lives);
}

void SpaceShooters::printScore(){
     mvwprintw(win,win_height-2,2,"SCORE: %d",score);
}

void SpaceShooters::printTimer(){

      if (!LeGame::inPause && !LeGame::inVictory && !LeGame::inGameOver)
            now = std::chrono::steady_clock::now();

      auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startPoint);
      int minutes = duration.count() / 60;
      int seconds = duration.count() % 60;

      mvwprintw(win, win_height-3, win_width - 7,"TIME");
      mvwprintw(win, win_height-2, win_width -7, "%02d:%02d", minutes, seconds);
}

void SpaceShooters::setStartPoint(){
      startPoint = std::chrono::steady_clock::now();
}

void SpaceShooters::setEnterPoint(){
      enterPoint = std::chrono::steady_clock::now();
}

void SpaceShooters::setExitPoint(){
      exitPoint = std::chrono::steady_clock::now();
}

void SpaceShooters::print(){
     // player -> bullets -> enemies : yazdırma sırası 
     player->draw();
     for(Enemy* e : enemies) e->draw();
     for(SuperEnemy* se: superEnemies) se->draw();
     for(PlayerBullet* pb : playerBullets) pb->draw();
     for(EnemyBullet* eb : enemyBullets) eb->draw();
     printLives();
     printScore();
     printTimer();
     wnoutrefresh(win);
}

void SpaceShooters::reset(){
     
     lives = 3;
     score = 0;

     gameFinished = false;
     
     player->reset();
     for(Enemy *e : enemies) e->reset();
     for(SuperEnemy* se : superEnemies) se->reset();
     enemyremains = enemies.size();

     playerBullets.clear();
     enemyBullets.clear();

     startPoint= std::chrono::steady_clock::now();
     enterPoint = exitPoint = startPoint;
}

const char* SpaceShooters::getName(){
     return "SPACE SHOOTERS";
}

int SpaceShooters::getScore() const {
    return score;
}

int SpaceShooters::timeBonus() {
      auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startPoint);

      if (duration.count() <= 90) return 50 * (90 - duration.count());
      else return 0;
}
     
// Entity Class: 
Entity::Entity(WINDOW* &win_,int y_, int x_){
     win = win_;
     y = y_;
     x = x_;
}

// Enemy Class:

Enemy::Enemy(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

SuperEnemy::SuperEnemy(WINDOW* &win_, int y_, int x_):Enemy(win_,y_,x_){
}

EnemyBullet* Enemy::shoot(){
     if (isDead) return nullptr;
     int random = rand();
     if (random % (60 - 2*(26 - SpaceShooters::enemyremains)) == 0){
        random = rand();
        if (random%10 == 0)
          return new EnemyBullet(win, this->y+height, this->x+(width/2), 3);
        else 
          return new EnemyBullet(win, this->y+height, this->x+(width/2), 1);
     }
     return nullptr;
}

bool Enemy::doesHit(PlayerBullet* pb){
     if ( x<=pb->getX() && pb->getX()<x+width && y<=pb->getY() && pb->getY()<y+height && !isDead){
           isDead = true;
            return true;     
     } 
     return false;
}

void Enemy::draw() const{
     if (isDead) return;
     int curr_y = y;
     for(const char* line : look) mvwprintw(win,curr_y++,x,"%s", line);
}

void SuperEnemy::draw() const {
     int curr_y = y;
      if (isDead) return;

     for(const char* line : look[lives-1]) mvwprintw(win,curr_y++,x,"%s", line);
}

void Enemy::reset(){
     isDead = false;
}

// SuperEnemy Class:

bool SuperEnemy::getIsDead(){
      return isDead;
}

void SuperEnemy::reset(){
      isDead = false;
      lives = 2;
}

bool SuperEnemy::doesHit(PlayerBullet* pb){
     if ( x<=pb->getX() && pb->getX()<x+width && y<=pb->getY() && pb->getY()<y+height && !isDead){
           if(--lives <= 0)
                 isDead = true;
            return true;     
     }
     return false;
}

// EnemyBullet Class:
EnemyBullet::EnemyBullet(WINDOW* &win_,int y_, int x_, int speedmult):Entity(win_,y_,x_){
    speed/=speedmult;
}

bool EnemyBullet::move(){
     y += 1;
     if (y > win_height-2) return true;
     return false;
}

void EnemyBullet::draw() const{
     mvwprintw(win,y,x,"%c",look);
}

int EnemyBullet::getSpeed(){
    return speed;
}

// Player Class:
Player::Player(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

bool Player::doesHit(EnemyBullet* eb) const {
     if ( x<=eb->getX() && eb->getX()<x+width && y<=eb->getY() && eb->getY()<y+height) return true;
     return false;
}

void Player::move(DIRECTIONS d){
     if (d == LEFT){
          if (x > 2) x-=speed; 
          if (x < 2) x = 2;
     }else{
          if (x + width < win_width-2) x += speed;
          if (x + width  >  win_width - 2) x = win_width - width -2;
     } 
}

PlayerBullet* Player::shoot(){
     return new PlayerBullet(win, this->y-1, this->x+(width/2));
}

void Player::draw() const{
     mvwaddch(win,y-1, x, wing);
     mvwaddch(win,y-1, x+6, wing);
     int curr_y = y;
     for(const char* line : look) mvwprintw(win,curr_y++,x,"%s", line);
}

void Player::reset(){
     y = 13;
     x = 34;
}

// PlayerBullet Class:
PlayerBullet::PlayerBullet(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

bool PlayerBullet::move(){
     y -= 1;
     if (y < 1) return true;
     return false;
}

void PlayerBullet::draw() const{
     mvwprintw(win,y,x,"%c",look);
}

int PlayerBullet::getSpeed(){
    return speed;
}

