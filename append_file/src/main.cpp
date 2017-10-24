#include "Append.h"

using namespace std;

int main(int argc, char **argv){
	Append *apnd = new Append(argc, argv);
	apnd->setup_files();
	apnd->setup_copy();
	delete apnd;
}