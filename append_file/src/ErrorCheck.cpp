#include "ErrorCheck.h"

using namespace std;

void ErrorCheck::error_check(bool argument, const char *message){
	if(argument){
		perror(message);
		exit(EXIT_FAILURE);
	}
}
void ErrorCheck::error_check(void *argument, const char *message){
	if(!argument){
		perror(message);
		exit(EXIT_FAILURE);
	}
}

void ErrorCheck::error_open(char *arg, bool is_des){
	cout << "opening "<< arg <<"..." << endl;
//	int open(char*, int, int);
	if(is_des) this->fd = open(arg, O_RDWR | O_CREAT | O_EXCL, 0644);
	else this->fd = open(arg, O_RDONLY);
	error_check((this->fd < 0), "open ");
}

void ErrorCheck::error_close(){
	cout << "closing..." << endl;
//	int close(int);
	error_check(
		close(this->fd),
		"close"
		);
}
//	void *mmap(void*, size_t, int, int, int, off_t);
void ErrorCheck::error_mmap(){
	cout << "mmapping..." << endl;
	this->mem = mmap(NULL, this->st.st_size, PROT_READ, MAP_SHARED, this->fd, 0);
	error_check(this->mem, "mmap");
}

void ErrorCheck::error_mmap(size_t size){
	cout << "mmapping..." << endl;
	this->mem = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, this->fd, 0);
	error_check(this->mem, "mmap");	
}

void ErrorCheck::error_stat(){
	cout << "stating..." << endl;
//	int fstat(int, struct stat*);
	error_check(
		fstat(fd, &this->st),
		"fstat"
		);
}

void ErrorCheck::error_trnc(off_t fd_size){
	cout << "truncating..." << endl;
//	int ftruncate(int, off_t);
	error_check(
		ftruncate(this->fd, fd_size),
		"ftruncate"
		);
}

//	int madvise(void*, size_t, int);
void ErrorCheck::error_advise(size_t mem_size){
	cout << "adviseing..." << endl;
	error_check(
		madvise(this->mem, mem_size, MADV_SEQUENTIAL),
		"madvise"
		); 
}
void ErrorCheck::error_advise(){
	cout << "adviseing..." << endl;
	error_check(
		madvise(this->mem, this->st.st_size, MADV_SEQUENTIAL),
		"madvise"
		); 
}
