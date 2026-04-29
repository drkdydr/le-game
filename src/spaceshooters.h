#ifndef SPACESHOOTERS_H
#define SPACESHOOTERS_H

#include <ncurses.h>
#include <vector>

class Entity;
class Enemy;
class SuperEnemy;
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
     int speed = 7; //tick count for bullet to move 1 unit
     char look = '*';
     public:
     EnemyBullet(WINDOW* &win, int y, int x, int speedmult);
     bool move();
     void draw() const;
     int getSpeed();
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
     // Enemy();
     Enemy(WINDOW* &win, int y, int x);
     EnemyBullet* shoot();
     void reset();
     void die();
     bool doesHit(PlayerBullet* pb) const;
     void draw() const;
};

// class SuperEnemy : public Enemy { // default enemy constructor gerekiyor diyor.
//     bool isDead = false;
//     public:
//     SuperEnemy(WINDOW* &win, int y, int x){
//         Enemy(win, y, x);
//     }
// };

class PlayerBullet : public Entity {
     friend class Enemy;
     const int speed = 5; //2
     char look = '^';
     public:
     PlayerBullet(WINDOW* &win, int y, int x);
     bool move();
     void draw() const;
     int getSpeed();
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
     
     inline static int enemyremains = 26;
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
          void process(int input); //bit based coding function result
          void print();
          void reset(); // will reset the game.
          const char* getName();
          int getScore() const;

    friend class Enemy; //aklıma daha iyi bir yol gelmedi (bu file 104, cpp 164)
};

#endif
