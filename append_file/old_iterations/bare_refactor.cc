#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <iostream>
#include <string.h>

using namespace std;
int size;
int openFileSource(char *arg){
	int fd = open(arg, O_RDONLY);
       	if (fd == -1) {
		perror("open source");
		exit(1);
	}
	return fd;
}
int openFileDestination(char *arg){
	int fd = open(arg, O_RDWR | O_CREAT | O_EXCL, 0644);
        if (fd == -1) {
                perror("open destintation");
                exit(1);
        }
	return fd;
}
void statErrorCheck(int fd, struct stat *st){
	if (fstat(fd, st)) {
               	perror("stat source");
               	exit(1);
	}
}
void* mmapErrorCheck(int fd, struct stat st){
        void* mem = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (mem == MAP_FAILED) {
       	        perror("map source");
               	exit(1);
       	}
	return mem;
}
int sourceLoop(int *fdList, struct stat *statList, void** mmapList){
	int sum = 0;
	for(int i=0; i<size; i++){
		statErrorCheck(fdList[i], &statList[i]);
		sum += statList[i].st_size;
		mmapList[i] = mmapErrorCheck(fdList[i], statList[i]);
		if(madvise(mmapList[i],statList[i].st_size,MADV_SEQUENTIAL)==0)
			cout<< "source file" << i+1 <<": OK" <<endl;
		else {
			cout<< "source file" << i+1 <<": FAIL" <<endl;
			exit(3);
		}
//		mem = mempcpy(mem, mmapList[i], statList[i].st_size);
	}
	return sum;
}
int main(int argc, char *argv[]){
	size = argc-2;
	int sourceArray[size];
	struct stat statArray[size];
	void* mmapArray[size];

	for(int i=0; i<size; i++) 
		sourceArray[i]=openFileSource(argv[i+1]);

	int d = openFileDestination(argv[size+1]);
	int sum = sourceLoop(sourceArray, statArray, mmapArray);

    	if (ftruncate(d, sum)) {
                perror("truncate destination");
                exit(1);
        }
        void* dp = mmap(NULL, sum, PROT_WRITE | PROT_READ, MAP_SHARED, d, 0);
        if (dp == MAP_FAILED) {
                perror("map destintation");
                exit(1);
        }
	if(madvise(dp,sum,MADV_SEQUENTIAL)==0)
		cout<< "destination file: OK" <<endl;
	void *mem;
	for(int i=0; i<size-1; i++){
		if(i==0) mem = mempcpy(dp, mmapArray[i], statArray[i].st_size);
		else mem = mempcpy(mem, mmapArray[i], statArray[i].st_size);
	}
	memcpy(mem, mmapArray[size-1], statArray[size-1].st_size ); 
	return 0;
}

