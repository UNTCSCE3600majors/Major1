#include "shell.h"
//Runs the shell in interactive mode.
void interactive()
{
	char *read = NULL; // string from input
	char *semicolon = NULL;//strtok var
	int exitstatus = 1;//checks if the user has told the program to exit
	read = malloc(sizeof(char)*100);
	int counter;

	while(exitstatus)
	{
		
		printf("prompt> ");
		fgets(read,100,stdin);//reads commands

		semicolon = strtok(read,"[;]\n");//checks for semicolons within each line read from the file

		while (semicolon != NULL)//while there are semicolons in the string left
		{
			if(!strcmp(semicolon,"exit"))//if the command is 'exit'
			{
				exitstatus = 0;//tells program to exit after the current line
				
			}
			else
			{	
				execute(semicolon);//give the command and flags to the execute command
				counter++;
				printf("\n");
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
	char *flags[100];//
	char *split = NULL;//strtok var
	int last;//holds the last arg position
	//printf("\nExecuting %s \n", command);


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
		execvp(flags[0], flags);//calls the command
		printf("EXECVP failed\n");//if the process could not be called, terminate child
		exit(getpid());
	}
	else
	{

	}
	return;
}
