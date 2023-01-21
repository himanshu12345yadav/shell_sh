#pragma once

#define SHELL_DEFAULT_PROMPT "\033[01;36mOS/SHELL:\033[34m"
#define RESET_SHELL_COLOR "\033[00m"
#define ERROR_PROMPT(command) ((char*) ("\033[1;31mOS/SHELL:\033[0:m" + command).c_str())
#define SHELL_START "\033c--------------SHELL_SH FEATURES-----------------\n\n1) Custom colored prompts like bash shell.\n2) Handles all errors gracefully.\n3) Common internel shell commands (Ex: exit , cd , clear) are supported.\n3) Can execute binaries available in the path environment variable.\n4) Supports complex pipes commands.\n5) Supports background task using (&) operators.\n"


char** vecToArr(std::vector<std::string>);
void simpleCmds(std::vector<std::string>, bool);
void pipedCmds(std::vector<std::vector<std::string>>);
