#include "Append.h"

using namespace std;

void Append::setup_vector(char **args, int it){
	if(it == nr_of_files) return;
	if(it == nr_of_files-1)
		err.insert(err.begin(),new ErrorCheck(args[nr_of_files-1], true));
	else err.push_back(new ErrorCheck(args[it], false));
	setup_vector(args, it+1);
}

off_t Append::source_loop(off_t length, int it){
	if(it == err.size()) return length;
	handle_source(this->data_points[it]);
	source_loop(length + this->data_points[it]->getSt().st_size, it+1);
}

void Append::setup_files(){
	handle_destination(this->data_points[0], source_loop((off_t) 0, 1));
}

void Append::handle_source(ErrorCheck *err){
	cout << "********SOURCE FILE " << err->getName() << "********" << endl;
	err->error_stat();
	err->error_mmap();
	err->error_advise();
}

void Append::handle_destination(ErrorCheck *err, off_t length){
	cout << "********SOURCE DESTINATION " << err->getName() << "********" << endl;
	err->error_trnc(length);
	err->error_mmap((size_t) length);
	err->error_advise((size_t) length);
}

void Append::handle_copy(){
	memcpy(
		this->mem,
		this->data_points[0]->getMmap(),
		this->data_points[0]->getSt().st_size
		);
}

void *Append::handle_copy(void *mem, int it){
	return	mempcpy(
				copy_loop(mem, it+1),
				this->data_points[it]->getMmap(),
				this->data_points[it]->getSt().st_size
				);
}

void *Append::init_copy(){
	return	mempcpy(
				this->data_points[0]->getMmap(),
				this->data_points[1]->getMmap(),
				this->data_points[1]->getSt().st_size
				);
}

void Append::setup_copy(){
	this->mem = copy_loop(init_copy(), 2);
	handle_copy();
}

void *Append::copy_loop(void *mem, int it){
	if(it == err.size()) return mem;
	mem = handle_copy(mem, it);
}