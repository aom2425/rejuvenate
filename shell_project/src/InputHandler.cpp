#include "InputHandler.h"

vector<char *>InputHandler::breakdown(char *input, const char *delim){
	vector<char *> list;
	cout << "breakdown" << endl;
	char *split = strtok(input, delim);
	while(split != NULL){
//		cout << split << endl;
		list.push_back(split);
		split = strtok(NULL, delim);
	}
	return list;
}

vector<char*> InputHandler::split_controller(char *set){
	cout << "split controller" << endl;
	if(strpbrk(set,";")) return breakdown(set, ";");
	if(strpbrk(set,"|")) return breakdown(set, "|");
	if(strpbrk(set,":")) return breakdown(set, ":");
	return breakdown(set, DELIMS);
}
string InputHandler::cmd_controller(char *set){
	cout << "command controller" << endl;
	if(strpbrk(set,">")) return "> found";
	if(strpbrk(set,"<")) return "< found";
	if(strpbrk(set,"*")) return "* found";
	if(strpbrk(set,"$")) return "$ found";
	if(strpbrk(set,"=")) return "= found";
	if(strpbrk(set,"!")) return "! found";
	return "*,!,<,>,=, nor $ was found";
}