#pragma once

#include "Convertions.h"
#include "ErrorCheck.h"
#include "InputHandler.h"
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
	~ Shell(){
		delete [] shell_instance;
	};
	
protected:
	static Shell *shell_instance;
	void die(){ kill = true; };
	void setPrompt(string newPrompt){ prompt = newPrompt; };
	void run_command(char **args);
	void set_essentials(char **cmds, vector<char*> vec);
private:
	string prompt;
	bool kill;
	char *input;
	void init();
	void get_next_line();
	void check_command(char **cmds);

	Shell(){ 
		prompt= "\n>>> ";
		kill = false; 
		printf("The \"exit\" command will exit shell.\n");
		get_next_line();
		init();
			
	};
};

