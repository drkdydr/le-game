#include "spaceshooters.h"
#include "game.h"
#include <cstdlib>
#include <ncurses.h>
#include <algorithm>
#include <unistd.h>

// todo:

// shoot chance of enemies should increase when enemy count decreases.
// immortal mode [optional]
// die animation (enemy/player) [optional]

SpaceShooters::SpaceShooters(WINDOW* &win_){
     
     tick = 0;
     win = win_;
     player = new Player(win,14,34);
     int idx = 0;
     Enemy* e;
     
     for(int x = 3; x<=67; x += 8){
          e = new Enemy(win,3,x);
          enemies.push_back(e);
     }
     for(int x = 7; x<=63; x += 8){
          e = new Enemy(win,5,x);    
          enemies.push_back(e);
     }
     for(int x = 3; x<=67; x += 8){
          e = new Enemy(win,7,x);    
          enemies.push_back(e);
     }
     
}

void SpaceShooters::process(int input){ //r1 r2 olayından çok memnun değilim ama iyi de duruyorlar.
     
     if (tick >= 100) tick = 0; //prevent overflow
     tick++;
     // if (tick % 50 != 0) return false; //tick rate for game (will be set more precisely)
     //tick'i buraya eklemek mallık 50 tane çalışmadan birinde çalışıyor.
     
     // flushinp(); // bunun oynayışını daha çok sevdim.
     // usleep(150000);
     
     for(Enemy* e : enemies){
          if(tick%10 == 0){
              EnemyBullet* eb = e->shoot();
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

          for(Enemy* e : enemies)
               if(e->doesHit(pb)){
                    score += killscore;
                    enemyremains--;
                    e->die();
                    playerBullets.erase(find(playerBullets.begin(), playerBullets.end(), pb));
               }
     }
     
     if (lives <= 0) { //gameover
         Game::inGameOver = true;
         return;
     }

     if (enemyremains <= 0) { //victory
         Game::inVictory = true;
         return;
     }

     switch(input){
          case 27 : case 'q':
               Game::inPause = true;
               break;
          case 'h' : case 'a' : case KEY_LEFT :
               player->move(LEFT);
               break;
          case 'l' : case 'd' : case KEY_RIGHT :
               player->move(RIGHT);
               break;
          case ' ' : case 'x': // shoot
               playerBullets.push_back(player->shoot());
               break;
          default :
               break;
     }


     print();
}

void SpaceShooters::printLives(){
     mvwprintw(win,win_height-2,2,"LIVES: %d",lives);
}

void SpaceShooters::printScore(){
     mvwprintw(win,win_height-3,2,"SCORE: %d",score);
}

void SpaceShooters::print(){
     // bullets -> player -> enemies : yazdırma sırası 
     for(PlayerBullet* pb : playerBullets) pb->draw();
     for(EnemyBullet* eb : enemyBullets) eb->draw();
     player->draw();
     for(Enemy* e : enemies) e->draw();
     printLives();
     printScore();
     wnoutrefresh(win);
}

void SpaceShooters::reset(){
     
     lives = 3;
     score = 0;
     
     player->reset();
     for(Enemy *e : enemies) e->reset();
     enemyremains = enemies.size();

     playerBullets.clear();
     enemyBullets.clear();
}

const char* SpaceShooters::getName(){
     return "SPACE SHOOTERS";
}

int SpaceShooters::getScore() const {
    return score;
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

bool Enemy::doesHit(PlayerBullet* pb)const{
     if ( x<=pb->x && pb->x<x+width && y<=pb->y && pb->y<y+height && !isDead) return true;
     return false;
}

void Enemy::die(){
     isDead = true;
}

void Enemy::draw() const{
     if (isDead) return;
     int curr_y = y;
     for(const char* line : look) mvwprintw(win,curr_y++,x,"%s", line);
}

void Enemy::reset(){
     isDead = false;
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
     if ( x<=eb->x && eb->x<x+width && y<=eb->y && eb->y<y+height) return true;
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

