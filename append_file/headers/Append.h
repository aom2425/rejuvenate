#pragma once

#include "ErrorCheck.h"
#include <vector>

using namespace std;

class Append {
public:
	Append(int nr_of_files, char **args){
		this->nr_of_files = nr_of_files;
		setup_vector(args, 1);
		this->data_points = err.data();
	};
	~Append(){};
	
	void setup_files();
	void setup_copy();

protected:
	int get_nr_of_files(){ return nr_of_files; };//returns the number of source files
	off_t source_loop(off_t length, int it);
	void *copy_loop(void *mem, int it);
	void setup_vector(char **args, int it);

private:
	int nr_of_files;//number of source files.
	vector<ErrorCheck*> err;
	ErrorCheck **data_points;
	void *mem;
	
	void handle_source(ErrorCheck *err);
	void handle_destination(ErrorCheck *err, off_t length);
	void *init_copy();
	void handle_copy();
	void *handle_copy(void *mem, int it);
};