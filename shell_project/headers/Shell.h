#pragma once

#include "Convertions.h"
#include "ErrorCheck.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>

#include "imports.h"

class Shell{

public:
	static Shell *instance(){
		if(!shell_instance) shell_instance = new Shell;
		return shell_instance;
	};
	~ Shell(){};
	
protected:
	static Shell *shell_instance;
	void die(){ kill = true; };
	void setPrompt(string newPrompt){ prompt = newPrompt; };
	void run_command(char **args);
	void loop_split(vector<char*> vec);

private:
	vector<char *> commands;
	vector<char *> path;
	vector<char *> pipes;
	vector<char *> multi_commands;
	string prompt;
	bool kill;

	void initial();
	void set_split_switch(char *input);
	vector<char *> breakdown(char *input, const char *delim);
	void check_command();
	Shell(){ 
		prompt= "\n>>> ";
		kill = false; 
		initial();	
	};
};
