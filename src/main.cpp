#include "legame.h"
#include <iostream>
#include <ncursesw/ncurses.h>

const char* version = "v.0.5";
const char* helpMessage = 
"Usage: le-game [OPTION]... \n\
TUI game center for Linux.\n\
\n\
     -h, --help          : displays this help screen.\n\
     -v, --version       : prints version\n\
\n";

void handleArg(char* &arg){
     //strcmp return 0 if string literals are equal
     //negative if str1 is less than str2
     //positive if str1 is bigger than str2
     
     if ( !strcmp(arg,"-h") || !strcmp(arg,"--help")){
          printf("%s",helpMessage);
          exit(0);
     }else if ( !strcmp(arg,"-v") || !strcmp(arg,"--version")){
          printf("Version %s\n",version);
          exit(0);
     }else{
          printf("le-game: invalid option -- '%s'\n",arg);
          exit(1);
     }
}

int main(int argc, char* argv[]){
     
     if (argc>1){
          for(int i = 1; i < argc; i++)
               handleArg(argv[i]);
     }
     
     LeGame g; 
     g.start();
     
     // char ch ;
     // ch = getch();
     
}
