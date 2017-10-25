#include "ErrorCheck.h"

void error_check(void *argument, const char *message){
	if(!argument){
		perror(message);
		exit(EXIT_FAILURE);
	}
}

void error_check(bool argument, const char *message){
	if(argument){
		perror(message);
		exit(EXIT_FAILURE);
	}
}

void ErrorCheck::exit_success(bool argument, const char *message){
	if(argument){
		printf("%s", message);
		exit(EXIT_SUCCESS);
	}
}

pid_t ErrorCheck::error_fork(){
	static pid_t pid = fork();
	error_check((pid < 0), "fork");
	return pid;
}
