#pragma once

#include "imports.h"
#include <vector>

class InputHandler{
public:
	static vector<char*> breakdown(char *input, const char *delim);
	static vector<char*> split_controller(char *set);
	static string cmd_controller(char *set);
};