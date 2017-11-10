#include <stdio.h>
#include <unistd.h>
int main() {
    char cwd[4096];
    fputs(getcwd(cwd,4096),stdout); // will print the current working directory
    fputs("\n",stdout); 
    chdir("/"); // change directory
    fputs(getcwd(cwd,4096),stdout); // print new working directory
    fputs("\n",stdout);
}
