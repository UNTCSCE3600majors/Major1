#include "shell.h"
//Runs the shell in interactive mode.
void interactive()
{
	char *read = NULL; // string from input
	char *semicolon = NULL;//strtok var
	int exitstatus = 1;//checks if the user has told the program to exit
	read = malloc(sizeof(char)*100);


	while(exitstatus)
	{
		printf("prompt> ");
		fgets(read,100,stdin);//reads commands

		semicolon = strtok(read,"[;|]\n");//checks for semicolons within each line read from the file

	int counter;
		while (semicolon != NULL)//while there are semicolons in the string left
		{
			if(!strcmp(semicolon,"exit"))//if the command is 'exit'
			{
				exitstatus = 0;//tells program to exit after the current line
				
			}
	
			else
			{
				execute(semicolon);//give the command and flags to the execute command
			}
			semicolon = strtok(NULL,"[; ]\n");
			
		}

		while(counter)
		{
			wait(NULL);
			counter--;
		}
		//wait( ( int *) 0);//waits for child to finish
    
	}
	free(semicolon);
    free(read);
	return;
}
//Runs the shell in Batch mode
void batch(char *filename)
{
	FILE *fstream; // opens a file pointer for handling input from files
	char *read;//temporary read in variable
	char *semicolon;//used to store seperate commands

	read = malloc(sizeof(char)*100);

	fstream = fopen(filename, "r"); // opens the batch file in read mode
	if(fstream == NULL)//check if file opened correctly; exit if no
	{
		printf("Opening file failed, exiting shell.\n");
		return;
	}
	while(fgets(read,100,fstream) != NULL)//read in from the batch file until it's empty
	{

		semicolon = strtok(read,"[;]\n");//checks for semicolons within each line read from the file

		while (semicolon != NULL)//while there are semicolons in the string left
		{
			execute(semicolon);//give the command and flags to the execute command
			semicolon = strtok(NULL,"[; ]\n");
		}

	}
	fclose(fstream); // close file
	return;
}
//call for each line of commands. Possibly replace spaces with -'s
void execute(char *command)
{

	int counter = 0;//checks the number of args passed to the function
	int counter2 =0; // runs through the flag array
	char *flags[100];//
	char *pipes[100];
	char *split = NULL;//strtok var
	int last;//holds the last arg position
	printf("\nExecuting %s \n", command);
	int fd[2];
	int stdin_cpy;
	int stdout_cpy;
	dup2(0,stdin_cpy);
	dup2(1,stdout_cpy);
	int pipmode = 0;
	int pipmode2 = 0;
	
	pipe(fd);
	
	if(fork() == 0)//creates a child process
	{
		split = strtok(command, " "); // searches the commands for flags
		while (split != NULL)
		{
			flags[counter]= split;
			counter++;
			split = strtok(NULL, " ");//checks for remaining spaces.
		}
		last = counter++;
		flags[last] = NULL; //sets null terminating character
		counter = 0;

	
		while(flags[counter2] != NULL)
		{
			pipes[counter2] = flags[counter2];
			printf("counter2 \n");
			if( pipmode2 == 1)
			{
				pipmode2 = 0;
				fd[1] = 0;
				dup2(fd[1],1);//makes stdin read from pipe
				close(fd[1]);
//				fd[1] = ;
				execvp(pipes[0], pipes);
			}
			if(!strcmp(flags[counter2], "|"))
			{
				pipmode = 1;
				pipmode2 = 1;
				pipes[counter2] = NULL;
				printf("The | op \n");
				fd[0] = 1;
				dup2(fd[0],0);//makes stdin read from pipe
				close(fd[0]);
//				fd[1] = ;
				execvp(pipes[0], pipes);	
				
			}
			counter2++;
				
		}
		if( pipmode == 0)
		{
			execvp(flags[0], flags);//calls the command
			printf("EXECVP failed\n");//if the process could not be called, terminate child
		}
		exit(getpid());
		
	}
	else
	{
		wait( ( int *) 0);//waits for child to finish
	}
	
	return;
}


/*
			pipelining = strtok(semicolon,"[|]\n"); // checks for pipelining
			if (pipelining != NULL)
			{
				while (pipelining != NULL)//while there are semicolons in the string left
				{
					if(!strcmp(pipelining,"exit"))//if the command is 'exit'
					{
						exitstatus = 0;//tells program to exit after the current line
					}
					pipmode = 1;
					pipecommands[tracker]= pipelining;
					tracker++;
					pipelining = strtok(NULL,"[| ]\n");
				}
				while(pipecommands[i+1] != NULL)
				{
					execute(pipecommands[i], pipmode);//give the command and flags to the execute command
					i++;
					pipmode = 0;
				}
			}
*/

/*
	if ( pipmode == '1')
	{
		int fd[2];
		pipe(fd);
		if(fork() == 0)//creates a child process
		{

			split = strtok(command, " "); // searches the commands for flags
			while (split != NULL)
			{
				flags[counter]= split;
				counter++;
				split = strtok(NULL, " ");//checks for remaining spaces.

			}
			last = counter++;
			flags[last] = NULL; //sets null terminating character
			dup2(fd[1], 1);
			close(fd[0]);
			close(fd[1]);
			execvp(flags[0], flags);//calls the command
			
			printf("EXECVP failed\n");//if the process could not be called, terminate child
			exit(getpid());
		}
		else
		{
			wait( ( int *) 0);//waits for child to finish
			

		}
	}else
	{
*/