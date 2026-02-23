#include "spaceshooters.h"
#include <ncurses.h>

SpaceShooters::SpaceShooters(WINDOW* &win_){
     win = win_;
     player = new Player(win,13,35);
     int idx = 0;
     Enemy* e;
     for(int x = 4; x<=68; x += 4){
          e = new Enemy(win,3,x);
          enemies.push_back(e);
     }
     for(int x = 8; x<=64; x += 4){
          e = new Enemy(win,5,x);    
          enemies.push_back(e);
     }
     for(int x = 4; x<=68; x += 4){
          e = new Enemy(win,7,x);    
          enemies.push_back(e);
     }
}
     
     bool SpaceShooters::process(int input){
          switch(input){
               case 27 : case 'q':
                    return true;
                    break;
               case 'k' : case 'w' : case KEY_UP :
                    
               default :
                    return false;
          }
     }
     
     void SpaceShooters::print(){
          for(PlayerBullet* pb : playerBullets) pb->draw();
     }
     
     void SpaceShooters::reset(){
     }

Entity::Entity(WINDOW* &win_,int y_, int x_){
     win = win_;
     y = y_;
     x = x_;
}

Enemy::Enemy(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

     void Enemy::draw() const{
          int curr_y = y;
          for(const char* line : look) mvwprintw(win,curr_y++,x,"%s", line);
     }

EnemyBullet::EnemyBullet(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

     void EnemyBullet::draw() const{
          mvwprintw(win,y,x,"%c",look);
          wnoutrefresh(win);
     }

Player::Player(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

     void Player::draw() const{
          int curr_y = y;
          for(const char* line : look) mvwprintw(win,curr_y++,x,"%s", line);
     }

PlayerBullet::PlayerBullet(WINDOW* &win_,int y_, int x_):Entity(win_,y_,x_){
}

     void PlayerBullet::draw() const{
          mvwprintw(win,y,x,"%c",look);
          wnoutrefresh(win);
     }

