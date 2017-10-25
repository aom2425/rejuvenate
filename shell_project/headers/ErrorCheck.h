#pragma once

#include "imports.h"

class ErrorCheck{

public:
	static pid_t error_fork();
	static void exit_success(bool argument, const char *message);
};
