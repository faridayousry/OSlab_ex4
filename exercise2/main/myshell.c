//NOTE: i have watched the following video as a guide so i have a briefly similar sequence: youtube.com/watch?v=pO1wuN3hJZ4
//however I have fully done my research and written, combined, and tested my code + algorithms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_LEN 1024

bool input= false;	//in case of input redirection
bool output = false;	//in case of output redirection

char in_source[50];	//to store the program from which we will get the source for input redirec
char out_dest[50];	//to store the destination program from which we will redirect the output to

int p_counter = 0;	//to track the number of pipes in a command
//int p_index;	//to track where the pipe was found

size_t read_command(char* cmd) {
	if (!fgets(cmd, BUFFER_LEN, stdin)) //get command and put it in line
		return 0; //if user hits CTRL+D break
	size_t length = strlen(cmd); // get command length
	if (cmd[length - 1] == '\n') cmd[length - 1] = '\0'; // clear new line
	return strlen(cmd); // return length of the command read
}
int build_args(char* cmd, char** argv) {
	char* token; //split command into separate strings
	token = strtok(cmd, " ");
	int i = 0;
	while (token != NULL) {// loop for all tokens
		argv[i] = token; // store token
		token = strtok(NULL, " "); // get next token

		if( is_pipe(argv[i]) == 1)
		{
			p_counter = p_counter + 1;
			//p_index = i;
			argv[i] = NULL;	//clear the pipe token
		}
		else
			if(strcmp(argv[i], "<") == 0)
			{
				input = true;
			}
			else
				if(strcmp(argv[i], ">") == 0)
				{
					output = true;
				}

		i++; // increment number of tokens
	}

	argv[i] = NULL; //set last value to NULL for execvp
	return i; // return number of tokens
}
void set_program_path(char* path, char* bin, char* prog) {
	memset(path, 0, 1024); // intialize buffer
	strcpy(path, bin); //copy /bin/ to file path
	strcat(path, prog); //add program to path
	for (int i = 0; i < strlen(path); i++) //delete newline
		if (path[i] == '\n') path[i] = '\0';
}

int is_pipe(char* input)
{
	if (strcmp(input, "|") == 0)	//if the "|" is found return 1, else return 0
		return 1;
	else return 0;
		
}

void do_pipe(char** arg1, char** arg2, int pipefunc[])
{
	//int pipefunc[2];	//1:write & 0:read
	//pid_t pid1, pid2;	//to store pid of 2 children for the 2 params	
	int pid = fork();

	if (pipe(pipefunc) < 0)
	{
		printf("pipe error invalid function");
		return;
	}

	//pid1 = fork();		//for the parameter before "|"
	if (pid < 0)
	{
		perror("pipe error in forking");
		return;
	}
	else
	if (pid == 0){		//child

		//close(pipefunc[zero]);	//we will not need the read (stdin)
		dup2(pipefunc[0], 0);	//redirect output to the pipe
		close(pipefunc[1]);	//close the write after dup
		execvp(arg2[0], arg2);
	}
	else

	if(execvp(arg1[0], arg1)<0)
	{
		fprintf(stderr, "Child process could not do execvp\n");
		exit(0);

	}
	else 

	

	pid2 = fork();		//for the parameter after "|"
	if (pid2 == 0){		//child
		close(pipefunc[1]);	//we will not need the write (stdout)
		dup2(pipefunc[0], STDIN_FILENO);	//redirect pipe as input to second param
		close(pipefunc[0]);	//close the read after dup

		if(execvp(args[1], args)<0)
		{
			fprintf(stderr, "Child process could not do execve\n");
			exit(0);

		}


	}


}



int main() {
	char line[BUFFER_LEN]; //get command line
	char* argv[100]; //user command
	char* bin = "/bin/"; //set path at bin
	char path[1024]; //full file path
	int argc; //arg count

	while (1) {
		printf("My shell>> "); //print shell prompt
		if (read_command(line) == 0)
		{
			printf("\n"); break;
		} // CRTL+D pressed
		if (strcmp(line, "exit") == 0) break; //exit
		argc = build_args(line, argv); // build program argument
		set_program_path(path, bin, argv[0]); // set program full path
		
		
		if (strcmp(line, "cd") == 0)
		{
			chdir(argv[1]);
		}
		else	
		{
			int pid = fork(); //fork child
			if (pid == 0) { //Child
			//if(execve(path, argv, 0)) // if failed process is not replaced
			   // then print error message
			//	{fprintf(stderr, "Child process could not do execve\n");
			//	exit(0);}
		
	
			if(execvp(*argv, argv)<0)
			{	
				fprintf(stderr, "Child process could not do execvp\n");
				exit(0);
			}


			if (is_pipe(line, argv) == 1)
			{
				do_pipe(argv[0], argv[1]); //****??
			}


			}	
			else wait(NULL); //Parent


		}	
	}
	return 0;

}
