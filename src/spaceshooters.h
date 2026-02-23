#ifndef SPACESHOOTERS_H
#define SPACESHOOTERS_H

#include <ncurses.h>
#include <vector>

enum DIRECTIONS {
     LEFT,
     RIGHT,
};

class Entity {
     protected:
     int x,y;
     WINDOW* win;
     public:
     Entity(WINDOW* &win, int y, int x);
};

class Enemy : public Entity {
     std::vector<const char*> look = {
          "|/-\\|",
          "|\\-/|"
     };
     public:
     Enemy(WINDOW* &win, int y, int x);
     void draw() const;
};

class EnemyBullet : public Entity {
     char look = '*';
     public:
     EnemyBullet(WINDOW* &win, int y, int x);
     void die();
     void draw() const;
};

class Player : public Entity {
     std::vector<const char*> look = {
          "|     |",
          "|\\/^\\/|",
          "' \\-/ '"
     };
     public:
     Player(WINDOW* &win, int y, int x);
     void move(DIRECTIONS d);
     void die();
     void draw() const;
};

class PlayerBullet : public Entity {
     char look = '^';
     public:
     PlayerBullet(WINDOW* &win, int y, int x);
     void draw() const;
};


class SpaceShooters{
     
     int lives = 3;
     
     // bool didWin = false;
     // bool didDie = false; // buralardan victory ya da game over screen'e falan atıcam.
     // isimlerini beğenmedim.
     
     std::vector<Enemy*> enemies;
     std::vector<EnemyBullet*> enemyBullets;
     
     Player* player;
     std::vector<PlayerBullet*> playerBullets;
     
     void printLives();
     void printEnemies();
     void printEnemyBullets();
     void printPlayerBullets();
     
     WINDOW* win;
     
     public:
          SpaceShooters(WINDOW* &win);
          bool process(int input);
          void print();
          void reset(); // will reset the game.
};

#endif