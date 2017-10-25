#include "Shell.h"

Shell *Shell::shell_instance = NULL;
main(int argc, char const **argv){
	Shell::instance();
	return 0;
}

