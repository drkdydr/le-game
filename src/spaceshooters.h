#ifndef SPACESHOOTERS_H
#define SPACESHOOTERS_H

#include <chrono>
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
      int x, y;
      WINDOW *win;

    public:
      Entity(WINDOW *&win, int y, int x);
      int getX() const { return x; }
      int getY() const { return y; }
};

class EnemyBullet : public Entity {
      int speed = 7; // tick count for bullet to move 1 unit
      char look = '*';

    public:
      EnemyBullet(WINDOW *&win, int y, int x, int speedmult);
      bool move();
      void draw() const;
      int getSpeed();
};

class Enemy : public Entity {
      std::vector<const char *> look = {"|/-\\|", "|\\-/|"};

    protected:
      bool isDead = false;
      const static int width = 5;
      const static int height = 2;

    public:
      static const int killscore;
      // Enemy();
      Enemy(WINDOW *&win, int y, int x);
      EnemyBullet *shoot();
      void reset();
      bool doesHit(PlayerBullet *pb);
      void draw() const;
};

class SuperEnemy : public Enemy {
      int lives = 2;

    public:
      static const int killscore;
      SuperEnemy(WINDOW *&win, int y, int x);
      void reset();
      bool getIsDead();
      bool doesHit(PlayerBullet *pb);
      std::vector<std::vector<const char *>> look = {{"\\/x\\/", "V|^|V"},{"X/x\\X", "V|^|V"}};
      void draw() const;
};

class PlayerBullet : public Entity {
      const int speed = 5; // 2
      char look = '^';

    public:
      PlayerBullet(WINDOW *&win, int y, int x);
      bool move();
      void draw() const;
      int getSpeed();
};

class Player : public Entity {
      const int width = 7;
      const int height = 3;

      const char wing = '|';

      const int speed = 3; // 3

      std::vector<const char *> look = {"|\\/^\\/|", "' \\-/ '"};

    public:
      Player(WINDOW *&win, int y, int x);
      void move(DIRECTIONS d);
      PlayerBullet *shoot();
      void reset();
      bool doesHit(EnemyBullet *eb) const;
      void die();
      void draw() const;
};

class SpaceShooters {

      int lives = 3;
      int score = 0;

      inline static int enemyremains = 26;
      int livescore = 500;

      bool gameFinished;

      int tick;
      std::chrono::time_point<std::chrono::steady_clock> start;
      std::chrono::time_point<std::chrono::steady_clock> now;
      std::chrono::time_point<std::chrono::steady_clock> lastPlayerShot;

      // bool didWin = false;
      // bool didDie = false; // buralardan victory ya da game over screen'e falan
      // atıcam. isimlerini beğenmedim.

      std::vector<Enemy *> enemies;
      std::vector<SuperEnemy *> superEnemies;
      std::vector<EnemyBullet *> enemyBullets;

      Player *player;
      std::vector<PlayerBullet *> playerBullets;

      void printLives();
      void printScore();
      void printTimer();
      void printEnemies();
      void printEnemyBullets();
      void printPlayerBullets();

      void printGameOver();
      void printVictory();

      WINDOW *win;

    public:
      SpaceShooters(WINDOW *&win);
      ~SpaceShooters();
      void process(int input); // bit based coding function result
      void print();
      void reset(); // will reset the game.
      const char *getName();
      int getScore() const;
      int timeBonus();

      friend class Enemy; // aklıma daha iyi bir yol gelmedi (bu file 104, cpp 164)
};

#endif
