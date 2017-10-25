#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <wordexp.h>
#include <regex.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <wordexp.h>

using namespace std;

//#define _GNU_SOURCE
#define DELIMS " \r\t\n"
#define WRITE 	1
#define READ	0
string paths[10];
string prompt = ">>> ";
/*      My sourses
        http://stackoverflow.com/questions/4788374/writing-a-basic-shell
        https://brennan.io/2015/01/16/write-a-shell-in-c/
	http://tldp.org/LDP/lpg/node11.html
	https://gist.github.com/parse/966049
	http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
*/

void my_path(string p, string arr[]);

vector<string> split_input(char *input, int del){
        vector<string> list;
        char *split;
        const char *delim;

        switch(del){
                case 1: delim = ";"; break;     //break multi line assignments down
                case 2: delim = DELIMS; break;  //break assignment into bite size piece
                case 3: delim = ":"; break;     //break paths down
		case 4: delim = "|"; break;	//break multi pipes down
        }
        split = strtok(input, delim);
        while(split != NULL){
//		cout << split << endl;
                list.push_back(split);
                split = strtok(NULL, delim);
        }
        return list;
}
void close_pipes(int pipes[]){
	close(pipes[0]);
	close(pipes[1]);
	close(pipes[2]);
	close(pipes[3]);
}
void run_pipe(char *args[][5], int anr){
	if(execvp(args[anr][0],args[anr]) < 0) perror("execvp");
	else exit(EXIT_SUCCESS);

}
/*
int check_cmd(char *cmds[]){
	string ck_lst[] = {"cat","cd","grep","ls","mkdir","mv","rm","rmdir"};
	char *bad[sizeof(ck_lst)];
	int bad_tst, ex = 0;
	for(int i = 0; i < sizeof(cmds); i++){
		bad_tst=0;
		for(int j = 0; j < sizeof(ck_lst); j++){
			if(strcmp(cmds[i],ck_lst[j])==0)continue;
			else bad_tst++;
			if(bad_tst==sizeof(ck_lst)) ex=-1;
		}
	} 
	return ex;
}
*/
vector<string> concat(vector<string> list_in, string cmd){
	vector<string> list_out;
	string thecat;
	while(!list_in.empty()){
		thecat = list_in.at(0)+"/"+cmd;
		list_out.push_back(thecat);
		list_in.erase(list_in.begin());
	}
	return list_out;
}
void mySystem(vector<string> argl){//char *args[]){
	int status;
//	string rn_p = "";
	pid_t pid = fork();
        if(pid == 0){
		vector<string> list, cmdL;
		char *env = getenv("PATH");
		char *env_p = new char[strlen(env)];
               	env_p = strcpy(env_p,env);
		list = split_input(env_p,3);
		list = concat(list,argl.at(0));
		int i;
		char *args[argl.size()+1];
		args[argl.size()]='\0';

		while(!list.empty()){
			if(strstr((char*) argl.at(0).c_str(),"/") != NULL)
				args[0]=(char *) argl.at(0).c_str();
			else
				args[0]=(char *) list.at(0).c_str();
	
			i = 1;
			while(args[i] != NULL){
				args[i]=(char *) argl.at(i).c_str();
				cout<<i<<" i and args: "<<args[i]<<endl;
				i++;
			}
			if(execv(args[0], args) < 0){
				list.erase(list.begin());
				continue;
			}else exit(EXIT_SUCCESS);
		}
		printf("Your wrong, no such command\n");
		exit(EXIT_FAILURE);
	}else if (pid < 0) perror("fork");
	else while (wait(&status) != -1);
}
void redirect(char *dir, char *file, int io){
	int status;
	pid_t pid = fork();
        if(pid == 0){
		int fd;
		cout << "dir " << dir << "file " << file << " io " << io << endl;
		vector<string> list, cmdL, argl;
		argl = split_input(dir, 2);
		if(io == WRITE){
			fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			cout << "write fd " << fd << endl;
			if(fd < 0) {
				perror("open");
				exit(1);
			}
			if(dup2(fd,1) < 0) perror("dup2 for writing");
		}else{
			fd = open(file, O_RDONLY);
			cout << "read fd " << fd << endl;
			if(dup2(fd,1) < 0) perror("dup2 for reading");
		}
		char *env = getenv("PATH");
		char *env_p = new char[strlen(env)];
		env_p = strcpy(env_p,env);
		list = split_input(env_p,3);
		list = concat(list,argl.at(0));
		int i;
		char *args[argl.size()+1];
		args[argl.size()]='\0';

		while(!list.empty()){
			args[0]=(char *) list.at(0).c_str();
			i = 1;
			while(args[i] != NULL){
				args[i]=(char *) argl.at(i).c_str();
				cout<<i<<" i and args: "<<args[i]<<endl;
				i++;
			}
			if(execv(args[0], args) < 0){
				list.erase(list.begin());
				continue;
			}else exit(EXIT_SUCCESS);
		}
		printf("Your wrong, no such command\n");
		exit(EXIT_FAILURE);
	}else if (pid < 0) perror("fork");
	else while (wait(&status) != -1);
}

void multi_pipe(char *args[][5], char *file, int io){
	int status;
	int pipes[4];
	pipe(pipes);
	pipe(pipes + 2);
	int pid  = fork();
	if(pid == 0){
		dup2(pipes[1],1);
		close_pipes(pipes);
		run_pipe(args,0);
	}else if(pid < 0) perror("fork");
	else{
		int pid2 = fork();
		if(pid2 == 0){
			dup2(pipes[0],0);
			dup2(pipes[3],1);
			close_pipes(pipes);
			run_pipe(args,1);
		}else if(pid2 < 0) perror("fork");
		else{
			int pid3 = fork();
			if(pid3 == 0){
				dup2(pipes[2],0);
				close_pipes(pipes);
				run_pipe(args,2);
			}else if(pid3 < 0) perror("fork");
		}
	}
	close_pipes(pipes);
	wait(&status);
}
void single_pipe(char *args[][5], char *file, int io){
	int status;
	pid_t pid = fork();
	if(pid == 0){
		int piped[2];
		pipe(piped);
		int spid = fork();
		if(spid == 0){
			dup2(piped[0],0);
			close(piped[1]);
			run_pipe(args,1);
		}else if(spid < 0) perror ("fork");
		else{
			dup2(piped[1],1);
			close(piped[0]);
			run_pipe(args,0);
		}
	}else if (pid < 0) perror("fork");
	else while (wait(&status) != -1);
}

char *change_dir(char * n_dir){
        char *symbol = strstr(n_dir, "~");
        char *home = getenv("HOME");
        char *cur_path = getenv("PWD");
	char *cur_path_cpy = new char[strlen(cur_path)];
	cur_path_cpy = strcpy(cur_path_cpy,cur_path);
	if(setenv("PWD", cur_path_cpy, 1) < 0) perror("setenv pwd");
	free(cur_path_cpy);
        if(symbol != NULL){
                char *next_dir = strtok(symbol, "~");
		if(next_dir != NULL){
                	char *dir = strcat(home, next_dir);
			const char *o_home = "/home/sregales";
                if(setenv("HOME", o_home, 1) < 0) perror("setenv home");
                if(setenv("PWD", dir, 1) < 0) perror("setenv pwd");
                return dir;
		}else{
	                if(setenv("PWD", home, 1) < 0) perror("setenv pwd");
			return home;
		}
        }
        return strcat(cur_path, n_dir);
}
void check_files(const char* pattern){
	int status;
	pid_t pid = fork();
	if(pid == 0){
		wordexp_t p;
		char **w;

		if(wordexp(pattern,&p,0) < 0) perror("wordexp");
		w = p.we_wordv;
		for (int i = 0; i < p.we_wordc; i++)
			printf("%s\n", w[i]);
		wordfree(&p);
		exit(EXIT_SUCCESS);
	}else if (pid < 0) perror("fork");
	else while (wait(&status) != -1);
}

main(){
        vector<string> aList, cList, bList;
        char *input, *split, *file, *dir, *word, *my_path;
        bool first = false, die = false;
	int i, j, k, io;

        const char* env_p = getenv("PATH");
        string epath = env_p;

	printf("\nThe \"q\" command will exit shell\n");
        while(!die){
                input = readline( prompt.c_str());
               	add_history(input);
		aList = split_input(input, 1);

                while(!aList.empty()){
			char *cmd = (char *) aList.at(0).c_str();
//			cout << cmd << endl;
			if(strstr(cmd,"*") != NULL){
				cout << "im in" << endl;
				strtok(cmd,"*");
				word = strtok(NULL,"*");
				string add = word;
				add = "*"+add;
				cout << add << endl;
				check_files((const char*) add.c_str());
				break;
			}
			if(strstr(cmd,"$") != NULL){
				cout << cmd << endl;
				split = strtok(cmd,"$");
				word = strtok(NULL,"\r");
				cList = split_input(split,2);
				cList.push_back(getenv(word));
				mySystem(cList);
				break;
			}
			if(strstr(cmd,"=") != NULL){
				word = strtok(cmd,"=");
				split = strtok(NULL,"\r");
				cout << split << endl;
				if(strcmp(word,"PS1") == 0){
					string newstr = split;
					prompt=newstr;
					break;
			        }else{
					if(setenv(word,split,1) < 0) perror("setenv");
					break;
				}
			}
			if(strstr(cmd,"!") != NULL){
				word = strtok(cmd,"!");
			}
			if(strstr(cmd,"|") != NULL){
//				cout << "split on pipe" << endl;
				cList =  split_input(cmd,4);
				int cSize = cList.size();
				cout << "cSize " << cSize << endl;
				char *pArgs[cSize][5];
				i = 0;
				while(!cList.empty()){
					word = (char *) cList.at(0).c_str();
//					cout << word << endl;
					if(strstr(word,">")){
						dir = strtok(word,">");
						file = strtok(NULL," ");
						io = WRITE;
					}else if(strstr(word,"<")){
						dir = strtok(word,"<");
						file = strtok(NULL," ");
						io = READ;
					}
//					cout << "cList size " << cList.size() << endl;
//					cout << "split "<< cList.at(0)<<" at space" << endl;
					split = strtok(word, DELIMS);
					k = 0;
					while(split != NULL){
//						cout << split << endl;
						cout << "pArgs[" << i << "][" << k << "]" << split << endl;
						pArgs[i][k] = split;
						split = strtok(NULL, DELIMS);
						k++;
					}
					pArgs[i][k] = '\0';
					cList.erase(cList.begin());
					i++;
				}
//				cout << "dir " << dir << "\tfile " << file << "\tio " << io << endl;
				if(cSize > 2)multi_pipe(pArgs,file, io);
				else single_pipe(pArgs,file, io);
			}
			if(strstr(cmd,">") != NULL){
				dir = strtok(cmd,">");
				cout << "dir " << dir << endl;
				file = strtok(NULL," ");
				cout << "file " << file << endl;
				redirect(dir,file,WRITE);
				break;
			}
			if(strstr(cmd,"<") != NULL){
				dir = strtok(cmd,"<");
				cout << "dir " << dir << endl;
				file = strtok(NULL," ");
				cout << "file " << file << endl;
				redirect(dir,file,READ);
				break;
			}else{
				cList = split_input(cmd, 2);
                       		aList.erase(aList.begin());
                        	char *args[cList.size()+1];
                       	 	args[cList.size()] = '\0';
                        	j = 0;
                       		while(args[j] != '\0'){
					args[j]=(char *) cList.at(j).c_str()+NULL;
               	                	j++;
               	        	}
				//cList.clear();
				
                        	if((strlen(args[0]) == 1) && (strcmp(args[0], "q") == 0)){
                                	die = true;
                                	break;
                        	}else if(strcmp(args[0], "cd") == 0) {
        	                        if(sizeof(args) > 1) my_path = change_dir(args[1]);
                	                else my_path = getenv("HOME");
                        	        cout << "my path " << my_path << endl;
                                	if(chdir(my_path) < 0) perror("chdir");
                                	continue;
                        	}
				mySystem(cList);
			}
/*
			else if (strcmp(args[0], ".") == 0){
			}
*/
		}
        }
        return 0;
}

