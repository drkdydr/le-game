#ifndef SPACESHOOTERS_H
#define SPACESHOOTERS_H

#include <ncurses.h>
#include <vector>

class Entity;
class Enemy;
class EnemyBullet;
class Player;
class PlayerBullet;

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

class EnemyBullet : public Entity {
     friend class Player;
     const int speed = 1; //1
     char look = '*';
     public:
     EnemyBullet(WINDOW* &win, int y, int x);
     bool move();
     void draw() const;
};

class Enemy : public Entity {
     bool isDead = false;
     const int width = 5;
     const int height = 2;
     std::vector<const char*> look = {
          "|/-\\|",
          "|\\-/|"
     };
     public:
     Enemy(WINDOW* &win, int y, int x);
     EnemyBullet* shoot();
     void reset();
     void die();
     bool doesHit(PlayerBullet* pb) const;
     void draw() const;
};

class PlayerBullet : public Entity {
     friend class Enemy;
     const int speed = 2; //2
     char look = '^';
     public:
     PlayerBullet(WINDOW* &win, int y, int x);
     bool move();
     void draw() const;
};

class Player : public Entity {
     const int width = 7;
     const int height = 3;
     
     const char wing = '|';
     
     const int speed = 3; //3
     
     
     std::vector<const char*> look = {
          "|\\/^\\/|",
          "' \\-/ '"
     };
     public:
     Player(WINDOW* &win, int y, int x);
     void move(DIRECTIONS d);
     PlayerBullet* shoot();
     void reset();
     bool doesHit(EnemyBullet* eb) const;
     void die();
     void draw() const;
};


class SpaceShooters{
     
     int lives = 3;
     int score = 0;
     
     int enemyremains = 26;
     int killscore = 200;
     int livescore = 500;
     
     int tick;
     
     // bool didWin = false;
     // bool didDie = false; // buralardan victory ya da game over screen'e falan atıcam.
     // isimlerini beğenmedim.
     
     std::vector<Enemy*> enemies;
     std::vector<EnemyBullet*> enemyBullets;
     
     Player* player;
     std::vector<PlayerBullet*> playerBullets;
     
     void printLives();
     void printScore();
     void printEnemies();
     void printEnemyBullets();
     void printPlayerBullets();
     
     void printGameOver();
     void printVictory();
     
     WINDOW* win;
     
     public:
          SpaceShooters(WINDOW* &win);
          ~SpaceShooters();
          int process(int input, bool& r0, bool& r1); //bit based coding function result
          void print();
          void reset(); // will reset the game.
          const char* getName();

};

#endif