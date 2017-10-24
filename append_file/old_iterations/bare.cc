#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;

void file_d(int des, int st){
        if (des == -1) {
                perror("open destintation");
                exit(1);
        }
	if (ftruncate(des,st)) {
                perror("truncate destination");
                exit(1);
        }
}
void source_p(void *sp){
        if (sp == MAP_FAILED) {
                perror("map source");
                exit(1);
        }
}
void file_o(int src, struct stat *st){
        if (src == -1) {
                perror("open source");
                exit(1);
        }
        if (fstat(src, st)) {
                perror("stat source");
                exit(1);
        }
}
int openFile(char* arg, struct stat st){
	int fd = open(arg, O_RDONLY);
	file_o(fd, &st);
	return fd;
}

int main(int argc, char *argv[]){
	struct stat st, st2;
        void *sp, *sp2, *dp;
        int s, s2, d;

	s = openFile(argv[1], st);
	s2 = openFile(argv[2], st2);
//	s = open(argv[1], O_RDONLY);
//	s2 = open(argv[2], O_RDONLY);
        d = open(argv[3], O_RDWR | O_CREAT | O_EXCL, 0644);
//	file_o(s, &st);
//	file_o(s2,&st2);
	cout << "source 1 size " << st.st_size << " source 2 size " << st2.st_size << " destination size " << (st.st_size+st2.st_size) <<  endl; 
	file_d(d,(st.st_size +st2.st_size));

	sp = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, s, 0);
	source_p(sp);
	sp2 = mmap(NULL, st2.st_size, PROT_READ, MAP_SHARED, s2, 0);
	source_p(sp2);

        dp = mmap(NULL, (st.st_size + st2.st_size), PROT_WRITE | PROT_READ, MAP_SHARED, d, 0);
        if (dp == MAP_FAILED) {
                perror("map destintation");
                exit(1);
        }
	
        if(madvise(sp,st.st_size,MADV_SEQUENTIAL)==0)
                cout<< "sp: we gucci" <<endl;
        if(madvise(sp2,st2.st_size,MADV_SEQUENTIAL)==0)
                cout<< "sp2: we gucci" <<endl;
        if(madvise(dp,(st.st_size+st2.st_size),MADV_SEQUENTIAL)==0)
                cout<< "dp: we gucci" <<endl;
	
	memcpy(mempcpy(dp, sp, st.st_size), sp2, st2.st_size);
        return 0;
}
