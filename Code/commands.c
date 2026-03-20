#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "simpleshell.h"

// File that contains all of the commands for the shell

/*Function to actually execute the commands. Some functions aken from sample shell given on Loop and turned into a function
  Author: Ian G Graham*/
int handle_command(char *args[]){

    if (!args[0])
        return 1;   // Empty line, continue

    char *input_file = NULL;    //
    char *output_file = NULL;
    int append = 0; //if append is set to 0, it will overwrite the file instead of appending to the end
    int background = 0; //if background is set to 0, it will run as a normal process, if it is 1, run in the background

    for (int i = 0; args[i] != NULL; i++) { //loop through all arguments
        if (!strcmp(args[i], "<")) { 
            input_file = args[i+1];     //same pattern for the next if statements, if there is a "<", the input file is the argument after it
            args[i] = NULL;     // change the symbol to a null value, cannot actually pass the symbol through the execvp command, it ends at the first NULL value, will explain more later
        }

        else if (!strcmp(args[i], ">")) {
            output_file = args[i+1];       //if there is a ">", the output file is the argument after it
            append = 0;     //set append to 0 (see above for what that does)
            args[i] = NULL; 
        }

        else if (!strcmp(args[i], ">>")) {
            output_file = args[i+1];
            append = 1;     //set append to 1
            args[i] = NULL;
        }
        else if (!strcmp(args[i], "&")) {
            background = 1;  //set background to 1
            args[i] = NULL;
        }
    }
    
    /*For these internal commands, i am using !strcmp. when using strcmp it doesnt evaluate as true or false, if the strings are the same it
    will return a 0 and not 0 ( or !0) is 1 therefore kind of evaluating as true. it is kind of the same as saying if strcmp(...) == 0 */


    /* Internal command: clear. compare the first argument to the string 'clr'. If the strings are the same, clear the screen using system */
    if (!strcmp(args[0], "clr")) {
        system("clear");
        return 1;
    }

    /* Internal command: quit. compare the first argument to the string 'quit'. if theyre the same, exit the program */
    if (!strcmp(args[0], "quit"))
        return 0;


    /* Internal command: pause. compare the first argument to the string 'pause'. If they're the same, display that the shell is paused until newline (enter) is typed*/
    if (!strcmp(args[0], "pause")) {
        printf("Paused, press enter to resume...");
        while (getchar() != '\n');
        return 1;
    }
    

    /* Internal command: cd(Change directory). compare the first argument to the string 'cd' If theyre the same, and if there is no argument after, print current working directory, or else change directory to the next argument*/
    if (!strcmp(args[0], "cd")) {
        int arg_count = 0;          // keeping count of the arguments, if 1, print current dir, if 2, change directory, if greater than 2, throw error
        while (args[arg_count] != NULL) 
            arg_count++;         // while loop for getting the amount of arguments
        char cwd[1024]; // allocating memory for the directory (1024 bytes)
        
        if (arg_count == 1) {
            if (getcwd(cwd, sizeof(cwd)) != NULL)      /* from https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program. */ 
                printf("%s\n", cwd);
            else
                perror("cd");
        }

        else if (arg_count == 2) {
            if (chdir(args[1]) != 0) {
                perror("cd");
        } else {
            if (getcwd(cwd, sizeof(cwd)) != NULL)
                setenv("PWD", cwd, 1);
        }
    }
            else {
                printf("cd: too many arguments\n");
            }
            return 1;
        }


        /* External commands */
        pid_t pid;  //this code was taken from 
        int rc;

        switch (pid = fork()) {     //creates a child process
            case -1:
                perror("fork");     //if the fork fails, print error
                return 1;

            case 0:
                if (input_file) {   //if the value of input_file == 1,replace standard input with the file name (basically, when writing commands normally, you are writing into standard input (stdin), in this case we will be getting the input from the file instead)
                    if (freopen(input_file, "r", stdin) == NULL) {  //if freopen fails, print error
                        perror("input redirection"); 
                        exit(1);
                    }
                }
                if (output_file) {  //same idea as above
                    if (append) {   //same idea as above, if append == 1, replace standard output with the file name and go into append mode
                        if (freopen(output_file, "a", stdout) == NULL) {
                            perror("output redirection");
                            exit(1);
                        }
                    }
                else {
                    if (freopen(output_file, "w", stdout) == NULL) { //replace standard output with the file name and go into write mode, (overwrite the file)
                            perror("output redirection");
                            exit(1);
                        }
                    }
                }

                /* Interal command: echo. compare the first argument to the string 'quit'. If they are the same, while the argument isnt null, print the argument, a new line and exit the program. */
                if (!strcmp(args[0], "echo")) {
                    int i = 1;
                    while (args[i] != NULL) {
                        printf("%s ", args[i]);
                        i++; 
                        }
                    printf("\n");
                    exit(0);
                }

                /* Internal command: environ. compare the first argument to the string 'environ'. If they're the same, list all of the environment strings*/
                if (!strcmp(args[0], "environ")) {
                    int i;
                    for (i = 0; environ[i] != NULL; i++) { //replace standard output with the file name and go into 
                        printf("%s\n", environ[i]);
                    }
                    exit(0);
                }

                /* Internal command: help. compare the first argument to the string 'help'. If they are the same, run the command 'more help.txt*/
                if (!strcmp(args[0], "help")) {
                    system("more manual/readme.txt");
                    exit(0);
                }

                if (!strcmp(args[0], "dir")) {
                    if (args[1] == NULL) {
                        execlp("ls", "dir", "-al", NULL);   //replace dir with ls -al and execute
                    }
                    else {
                        execlp("ls", "dir", "-al", args[1], NULL); //replace dir with ls -al and the next argument and execute
                    }
                    perror("dir");
                    exit(1);    
                }

                setenv("parent", getenv("shell"), 1); //sets environment variable to parent
                execvp(args[0], args); // runs program
                perror("execvp");
                exit(1);
            
            default:
                if(!background) {
                waitpid(pid, &rc, 0);   //waits for the child process to finish
            }
        }
        return 1;
    }
