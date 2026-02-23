#ifndef COMINGSOON_H
#define COMINGSOON_H

#include <ncurses.h>
#include <vector>

class ComingSoon {
     
     const std::vector<const char*> comingsoon_logo = {
          " _____ _____ _____ _____ _____ _____    _____ _____ _____ _____ ",
          "|     |     |     |     |   | |   __|  |   __|     |     |   | |",
          "|   --|  |  | | | |-   -| | | |  |  |  |__   |  |  |  |  | | | |",
          "|_____|_____|_|_|_|_____|_|___|_____|  |_____|_____|_____|_|___|"
     };
     const char* message = "This game is not currently available wait for future updates!";
     
     public:
          const char* getName();
          bool process(WINDOW* &win, int input);
          void print(WINDOW* &win);
          void reset();
};

#endif