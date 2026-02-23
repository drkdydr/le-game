#include "spaceshooters.h"
#include "game.h"
#include <ncurses.h>
#include <algorithm>
#include <unistd.h>

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

bool SpaceShooters::process(int input){ //inPause değerini döndürüyor 
     
     // tick++;
     // if (tick % 50 != 0) return false; //tick rate for game (will be set more precisely)
     //tick'i buraya eklemek mallık 50 tane çalışmadan birinde çalışıyor.
     
     // if (lives <= 0) printGameOver();
     
     // if (enemies.size() == 0) printVictory();
     
     flushinp(); // bunun oynayışını daha çok sevdim.
     usleep(100000);
     // flushinp();
     
     switch(input){
          case 27 : case 'q':
               return true;
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
     
     for(EnemyBullet* eb : enemyBullets){
          if(eb->move()) enemyBullets.erase(find(enemyBullets.begin(),enemyBullets.end(),eb)); // delete bullet if exceed border.
          // player->doesHit(eb);
          if (player->doesHit(eb)){
               lives--;
               enemyBullets.erase(find(enemyBullets.begin(), enemyBullets.end(), eb));
          }
     }
     
     for(PlayerBullet* pb : playerBullets){
          if(pb->move()) playerBullets.erase(find(playerBullets.begin(),playerBullets.end(),pb));
          for(Enemy* e : enemies)
               if(e->doesHit(pb)){
                    e->die();
                    playerBullets.erase(find(playerBullets.begin(), playerBullets.end(), pb));
               }
     }
     
     print();
     
     // for(EnemyBullet* eb : enemyBullets){
     // }
     
     
     
     
     return false;
}

void SpaceShooters::print(){
     // bullets -> player -> enemies : yazdırma sırası 
     for(PlayerBullet* pb : playerBullets) pb->draw();
     for(EnemyBullet* eb : enemyBullets) eb->draw();
     player->draw();
     for(Enemy* e : enemies) e->draw();
}

void SpaceShooters::reset(){
     
     int lives = 3;
     
     player->reset();
     for(Enemy *e : enemies) e->reset();
     for(EnemyBullet* eb : enemyBullets){
          enemyBullets.erase(find(enemyBullets.begin(), enemyBullets.end(), eb));
          // delete eb;
     }
     for(PlayerBullet* pb : playerBullets){
          playerBullets.erase(find(playerBullets.begin(), playerBullets.end(), pb));
          // delete pb;
     }
}

const char* SpaceShooters::getName(){
     return "SPACE SHOOTERS";
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
     wnoutrefresh(win);
}

void Enemy::reset(){
     isDead = false;
}

// EnemyBullet Class:
EnemyBullet::EnemyBullet(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

bool EnemyBullet::move(){
     y += speed;
     if (y > win_height-2) return true;
     return false;
}

void EnemyBullet::draw() const{
     mvwprintw(win,y,x,"%c",look);
     wnoutrefresh(win);
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
     return new PlayerBullet(win, this->y, this->x+3);
}

void Player::draw() const{
     mvwaddch(win,y-1, x, wing);
     mvwaddch(win,y-1, x+6, wing);
     int curr_y = y;
     for(const char* line : look) mvwprintw(win,curr_y++,x,"%s", line);
     wnoutrefresh(win);
}

void Player::reset(){
     y = 13;
     x = 34;
}

// PlayerBullet Class:
PlayerBullet::PlayerBullet(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

bool PlayerBullet::move(){
     y -= speed;
     if (y < 1) return true;
     return false;
}

void PlayerBullet::draw() const{
     mvwprintw(win,y,x,"%c",look);
     wnoutrefresh(win);
}

