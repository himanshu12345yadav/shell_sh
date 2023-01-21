#include<iostream>
#include<vector>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include "utils.h"

using namespace std;

char** vecToArr(vector<string> vec) {
	// implements a function to convert vector to a char* array for exec system calls
	char** arr = new char*[vec.size() + 1];
	for (int i = 0 ; i < vec.size() ; i++) {
		const char* origArg = vec.at(i).c_str();
		char *arg = new char[strlen(origArg) + 1];
		strcpy(arg, origArg);
		arr[i] = arg;
	}
	arr[vec.size()] = (char*) NULL; // Last argument should be NULL in exec
	return arr;
}



void simpleCmds(vector<string> command , bool bgFlag) {
	// Handles basic commands like cd , exit , or simple commands
	if (command[0] == "cd")
	{
		// change directory system call
		char * dir = (char*) command[1].c_str();
		if (chdir(dir) == -1) {
			perror("CHDIR ERROR: ");
		}
	}
	else if (command[0] == "exit") {
		// success exit system call
		exit(EXIT_SUCCESS);
	}
	else {
		// Clone a process and execute the commands
		char** cmd = vecToArr(command);
		pid_t pid = fork();

		if (pid == 0) {
			// inside the child process
			if (execvp(cmd[0] , cmd) == -1) {
				perror(ERROR_PROMPT(command[0]));
			}
		}
		else {
			// inside the parent process
			// wait for the child process to complete if bg flag is false
			delete[] cmd; // clear the memory from the parent process
			if (bgFlag) {
				cout << "[1]\t" << pid << endl;
			}
			else {
				wait(NULL);
			}
		}
	}
}


void pipedCmds(vector<vector<string>> parsed) {
	int commandSize = parsed.size();
	int fd[2];// old file descripters
	int newfd[2]; // new file descripters

	for (int i = 1 ; i <= parsed.size() ; i++) {
		if (i != commandSize) {
			// Make new pipes until the last command is left
			pipe(newfd);
		}
		vector<string> command = parsed.at(i - 1);
		char** cmd = vecToArr(command);
		// make a child process to execute the command
		pid_t pid = fork();
		if (pid == -1) perror("FORK ERROR: ");
		if (pid == 0) {
			// Inside the child process
			if (i > 1) {
				// Commands after first command
				dup2(fd[0] , STDIN_FILENO);
				close(fd[0]);
				close(fd[1]);
			}
			if (i < commandSize) {
				// commands before the last command
				dup2(newfd[1] , STDOUT_FILENO);
				close(newfd[0]);
				close(newfd[1]);
			}
			// execute the command
			if (execvp(cmd[0] , cmd) == -1) {
				perror(ERROR_PROMPT(command[0]));
				exit(EXIT_FAILURE);
			}
		}
		else {
			// Inside the parent process
			// close the old file descripters
			close(fd[0]);
			close(fd[1]);
			// copy to the new file descripters
			fd[0] = newfd[0];
			fd[1] = newfd[1];
			waitpid(pid , NULL , 0);// wait for the child process to complete
			delete[] cmd; // clear the dynamic memory from the parent process
		}
	}
}