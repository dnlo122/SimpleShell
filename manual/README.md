This is a simple shell with a few internal commands but it can also support external commands.


How to run:
1. Go to stage 1 and run make.
2. Then run ./bin/simpleshell or ./bin/simpleshell (name of batchfile).txt if you have a batch file
the internal commands are:
clr: clears the screen
quit: exits the shell
echo: prints out whats after the 'echo'
help: takes you to this page
pause: pauses the shell until enter is pressed
environ: lists the environment strings
cd: changes directories, if nothing is typed ofter cd, prints current working directory
dir: lists the files and/or directories in current directory if there is no directory after the 'dir'
    or else it lists the files/directories of the directory entered after 'dir'

For I/O redirection, <, > and >> are used for programs to read input from files and output to files instead of the terminal
    < sets the input file
    > sets the output file to overwrite mode
    >> sets thes the output file to append mode


The shell also background execution using &. When a command ends with & it is executed without blocking the shell.

The shell can find its own path and stores it in the environment variable SHELLPATH (run environ to see it).
This allows the shell and any child processes to know the location of the shell.

Name: Daniel Omoniyi
Student number: 24396
I, Daniel Omoniyi acknowledge DCU's academic integrity policy.
