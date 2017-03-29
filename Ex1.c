/*
 ============================================================================
 Name        : Ex1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
*/

// Jonathan Geva //
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argv, char* argc[]) {
	pid_t pid, pid2;
	pid = fork(); // creating a son - for compiling the program
	int status;
	if (pid == 0) // checking if son was created
			{
		execl("/usr/bin/gcc", "gcc", "-o", "yosef", argc[1], NULL); // compiling process - gcc
	} else {
		wait(&status); // waiting for son process to end
		if (status != 0) // checking if compilation failed
				{
			write(1, "your score:0,COMPILATION_ERROR\n", 31);
			exit(1);
		}
		pid2 = fork(); // creating other son - for running the program
		if (pid2 == 0) // checking if son was created
				{
			execl("./yosef", "yosef", argc[2], argc[3], NULL); // executing the program
		} else {
			sleep(5); // waiting 5 seconds
			int quit;
			quit = waitpid(pid2, &status, WNOHANG); // getting the status of pid2
			if (quit == 0) // running and took over 5 seconds
					{
				kill(pid2, SIGTERM); // killing pid2
				write(1, "your score:0,TIMEOUT\n", 21);
				exit(1);
			}
			if (WIFSIGNALED(status)) // runtime error
			{
				write(1, "your score:0,EXCEPTION\n", 23);
				exit(1);
			}
			if (WIFEXITED(status)) // compiled and finished running
				{
				status=WEXITSTATUS(status);
				switch (status) // status has the exit code
				{
				case 14: {
					write(1, "your score:100,GREAT_JOB\n", 25);
					break;
				}
				case 16: {
					write(1, "your score:80,NOT_GOOD_OUTPUT\n", 30);
					break;
				}
				default: // status has different value (not 14 or 16)
				{
					char message[1024] = { '\0' };
					sprintf(message, "%d\nyour score:0,BAD_OUTPUT\n", status);
					write(1, message, strlen(message));
					break;
				}
				}
				exit(0);
			}
		}
	}
	return 0;
}
