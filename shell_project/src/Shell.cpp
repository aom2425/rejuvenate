#include "Shell.h"

void Shell::run_command(char **args){
    int status;
    pid_t pid = ErrorCheck::error_fork();
    if(pid == 0) execv(args[0], args);
    else while (wait(&status) != -1);
}

void Shell::get_next_line(){
	this->input = readline(Convertions::convert_string_down(prompt));
	add_history(input);
}
vector<char*> clean_memory(vector<char*> vec){
	vec.erase(vec.begin());
	return vec;
}

void Shell::init(){
	while(!kill){
		cout << "kill loop" << endl;
		vector<char*> container = InputHandler::split_controller(input);
		while(!container.empty()){
			cout << container.at(0) << endl;
			vector<char*> item = InputHandler::split_controller(input);
			while(!item.empty()){
				cout << item.at(0) << endl;
				cout << InputHandler::cmd_controller(item.at(0)) << endl;
				item = clean_memory (item);
			}
			container = clean_memory(container);
		}
		get_next_line();
	}
}
void Shell::set_essentials(char **cmds, vector<char*> vec){
	cout << "run multi instrct" << endl;
//	cmds = InputHandler::breakdown(vec.data()[0]).data();
	check_command(cmds);
}

void Shell::check_command(char **cmds){
	ErrorCheck::exit_success(
		!strcmp(cmds[0], "exit"),
		"Goodbye, till next time.\n"
		);
}