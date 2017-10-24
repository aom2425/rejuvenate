#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//open(2)
#include <unistd.h>//ftruncate()
#include <sys/mman.h>//mmap()
#include <sys/stat.h>//open(2)
#include <sys/types.h>//open(2)
#include <iostream>
#include <string.h>

class ErrorCheck {
	/*	Class
		main purpose is to hold all the method that deals with error checking
		failure of any of the following calls will result in a halt in the
		program.
		-open 2
		-close
		-mmap
		-ftruncate
		-fstat
		-madvise */

public:
	/*	Constructor
		takes in an charecter pointer (filename) and a boolean
		arg is passed as the Errorcheck object variable name
		error_open is called with arg and des as the paramaters */
	ErrorCheck(char *arg, bool des){
		this->name = arg;
		error_open(arg, des);
	};
	/*	Deconstructor
		when object is deleted clean up space by closing the fd */
	~ErrorCheck(){ error_close(); };

	/*	Setter and getters
		-st holds struct stat
		-fd holds file descriptor of the the file being past through
		-mem holds pointer that points to a blank memory allocation */
	void setSt(struct stat st){ this->st = st; };
	struct stat getSt(){ return st; };
	void setFd(int fd){ this->fd = fd; };
	int getFd(){ return fd; };
	char *getName(){ return name; };
	void setMmap(void *mem){ this->mem = mem; };
	void *getMmap(){ return mem; };
	
	/*	error_open
		takes a char* (filenames) and boolean (is_destination) as parameter
		-char* is used in open (2) that returns a int; more info man 2 open
		-bool is used to check if the incoming char* is a destination file
		on success error_open set the class variable fd
		 */
	void error_open(char *arg, bool is_des);
	void error_close();//closes the class variable fd that was created by open 

	/*	error_stat
		uses fd and uninitialize st class variable and subsequently, 
		on success of fstat, initializes st*/
	void error_stat();

	/*	error_mmap & error_mmap(size_t)
		uses st, and fd class variables to set the mem class variable on success
		(size_t) method ignores the st variable and instead passes the parameter
		through to be resolve by mmap.
		*/
	void error_mmap();
	void error_mmap(size_t size);

	/*	error_trunc
		off_t : total size in bytes
		uses fd_size argument to truncate the fd to that exact size of bytes*/
	void error_trnc(off_t fd_size);

	/*	error_advise & error_advise(size_t)
		advises the kernel to use the class variable mem as sequential
		(size_t) does the same but with the length spesified by the user */
	void error_advise();
	void error_advise(size_t mem_size);

protected:
	void error_check(bool argument, const char *message);
	void error_check(void *argument, const char *message);

private:
	char *name;//		file name
	int fd;//			file descriptor
	struct stat st;//	holds metadata from fd
	void *mem;//		the virtual address space
};