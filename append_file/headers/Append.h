#pragma once

#include "ErrorCheck.h"
#include <vector>

using namespace std;
/*Append should inherite ErrorCheck*/
class Append {
public:
	Append(int nr_of_files, char **args){
		this->nr_of_files = nr_of_files;
		setup_vector(args, 1);
		this->data_points = err.data();
	};
	~Append(){};
	
	void setup_files();//calls handle_destination and source_loop appropriately 
	void setup_copy();//calls init_copy, handle_copy and copy_loop appropriately

protected:
	int get_nr_of_files(){ return nr_of_files; };//returns the number of files; includes the destination file
	off_t source_loop(off_t length, int it);//runs handle_source for as many times as there is source files 
	void *copy_loop(void *mem, int it);//runs handle_copy(void*, int) from the second source file to the last file in the vector
	void setup_vector(char **args, int it);//pushes a new ErrorCheck object to the back of the vector for as many files that is past as arguments

private:
	int nr_of_files;//number of source files.
	vector<ErrorCheck*> err;//holds an Error object for each file in the system 
	ErrorCheck **data_points;//points to the first object of the vector err (destination object)
	void *mem;//holds the poly-mempcpy structure created by copy_loop
	
	/*	handle_source
		takes in a ErrorCheck object and runs passes them through 3 ErrorChecks:
		error_stat
		error_mmap
		error_advise*/
	void handle_source(ErrorCheck *err);
	/*	handle_destination
		similar to handle_source but uses the off_t length to pass through the ErrorChecks
		and since this is the destination file we will truncate it to the proper byte size
		so instead of error_stat we use error_trunc*/
	void handle_destination(ErrorCheck *err, off_t length);
	/*	init_copy
		sets up the most inner mempcpy this includes 
		destination virtual address space
		first file virtual address space and size in bytes*/
	void *init_copy();
	/*	handle_copy
		runs memcpy with the class variable mem, thats been assable by copy_loop,
		the virtual adrress and size, in bytes, of destination file */
	void handle_copy();
	/*	handle_copy
		similar to init_copy, but instead of destination and first file virtual space, 
		it calls copy_loop and passes the current file virtual space*/
	void *handle_copy(void *mem, int it);
};