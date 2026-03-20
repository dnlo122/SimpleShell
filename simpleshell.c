#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "simpleshell.h"            // used to link this file to commands.c via simpleshell.h


/*Note that some of this code was repurposed from the code given on loop, I just changed up some things and*/

/*Function to tokenize input into args array. Taken from sample shell given on Loop and turned into a function
  Author: Ian G Graham*/
void tokenize_input(char *buf, char *args[]){
    char **arg = args;

    *arg++ = strtok(buf, SEPARATORS);

    while ((*arg++ = strtok(NULL, SEPARATORS)));
}

/*Function to actually execute the commands. Taken from sample shell given on Loop and turned into a function
  Author: Ian G Graham*/


int main(int argc, char **argv){
    char buf[MAX_BUFFER];
    char *args[MAX_ARGS];
    int running = 1;                //taken from sample shell
    FILE *input = stdin;

    char shell_path[PATH_MAX];          // finds the path for the shell and stores it in the environment variable shell
    if (realpath(argv[0], shell_path) != NULL)
        setenv("shell", shell_path, 1);


    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            perror("Batch file"); //prints the error
            exit(1);
        }
    }

    while (running && !feof(input)) {

        if (input == stdin) {
        /* Prints prompt (current directory) */
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) // same idea for cd, if getcwd works print the prompt
                printf("\033[36m%s>\033[0m ", cwd); // prints the prompt but i wanted it to be blue
            else
                perror("getcwd");
        }

            /* Reads input */
            if (fgets(buf, MAX_BUFFER, input)) {
                tokenize_input(buf, args);
                running = handle_command(args);
            } else {
                break;
            }
        }
        if (input != stdin)
            fclose(input);

    return 0;

    }

    
