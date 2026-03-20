#ifndef SIMPLESHELL_H       //Taken from https://stackoverflow.com/questions/1653958/why-are-ifndef-and-define-used-in-c-header-files for understanding
#define SIMPLESHELL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#define MAX_BUFFER 1024             // max line buffer
#define MAX_ARGS 64                 // max number of arguments
#define SEPARATORS " \t\n"          // token separators
extern char **environ;


void tokenize_input(char *buf, char *args[]);
int handle_command(char *args[]);
#endif


