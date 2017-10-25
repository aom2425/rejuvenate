#include "Shell.h"

void Shell::run_command(char **args){
    int status;
    pid_t pid = ErrorCheck::error_fork();
    if(pid == 0) execv(args[0], args);
    else while (wait(&status) != -1);
}

void Shell::initial(){
	printf("The \"exit\" command will exit shell.\n");
	while(!kill){
		char *input = readline(Convertions::convert_string_down(prompt));
		add_history(input);
		set_split_switch(input);
		loop_split(multi_commands);
//		commands = breakdown(input);
//		check_command();
	}
}
void Shell::loop_split(vector<char*> vec){
	if(vec.empty()) return;
	set_split_switch(vec.data()[0]);
	vec.erase(vec.begin());
	loop_split(vec);
}

void Shell::set_split_switch(char *input){
	if(strstr(input, ";")) this->multi_commands = breakdown(input, ";");
	else if(strstr(input, "|")) this->pipes = breakdown(input, "|");
	else if(strstr(input, ":")) this->path = breakdown(input, ":");
	else this->commands = breakdown(input, DELIMS);
}

vector<char *> Shell::breakdown(char *input, const char *delim){
	vector<char *> list;
	char *split = strtok(input, delim);
	while(split != NULL){
		cout << split << endl;
		list.push_back(split);
		split = strtok(NULL, delim);
	}
	return list;
}

void Shell::check_command(){
	char **commands_pointer = commands.data();
	ErrorCheck::exit_success(
		!strcmp(commands_pointer[0], "exit"),
		"Goodbye, till next time.\n"
		);
}