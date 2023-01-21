#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>
#include<vector>
#include<limits.h>

#include "utils.h"

using namespace std;


int main(int argc , char* argv[]) {
	cout << SHELL_START << endl;
	string command;
	string delimeter = " ";// Setting a empty string as the delimeter
	string pipeDelimiter = "|"; // setting the pipe delimeter
	bool bgFlag;

	while (true) {
		cout << SHELL_DEFAULT_PROMPT << getcwd(NULL , PATH_MAX) << RESET_SHELL_COLOR << "$ ";// show the prompt string to the user

		getline(cin , command);
		bgFlag = false;
		if (command.empty()) {
			// check if the empty input is given
			continue;
		}

		vector<vector<string>> parsed;
		char* pipeToken;
		char* leftoverPipes = (char*) command.c_str();


		while ((pipeToken = strtok_r(leftoverPipes , pipeDelimiter.c_str() , &leftoverPipes))) {
			vector<string> subCommands;
			char* token;
			char* leftover = pipeToken;
			while ((token = strtok_r(leftover , delimeter.c_str() , &leftover))) {
				string str(token);
				subCommands.push_back(str);
			}
			parsed.push_back(subCommands);
		}
		// check if bg flag is set
		if (parsed.back().back() == "&") {
			parsed.back().pop_back();
			bgFlag = true;
		}

		if (parsed.size() == 1) {
			// Handle basic internal , simple commands
			simpleCmds(parsed.front() , bgFlag);
		}
		else {
			// Handle Pipes commands here
			pipedCmds(parsed);
		}
	}
	return 0;
}

