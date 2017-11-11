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

		while (semicolon != NULL)//while there are semicolons in the string left
		{
			if(strcmp(semicolon,"exit") == 0)//if the command is 'exit'
			{
				exit(0);//tells program to exit after the current line
			}
			else
			{
				execute(semicolon);//give the command and flags to the execute command
			}
			semicolon = strtok(NULL,"[; ]\n");
		}
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
	char stuff[600];
	int i=0;
	int counter = 0;//checks the number of args passed to the function
	char *flags[100];//
	char *split = NULL;//strtok var
	int last;//holds the last arg position
	printf("\nExecuting %s \n", command);
	char *third;
	char *string;

	if(fork() == 0)//creates a child process
	{
		split = strtok(command, " "); // searches the commands for flags
		while (split != NULL)
		{
			flags[counter]= split;
			counter++;
			split = strtok(NULL, " ");//checks for remaining spaces.
		}
		//looks for cd input
		if(strcmp(flags[0],"cd") == 0)
		{
			//executes cd input
			chdir(flags[1]);
			return;
		}
		else if(strcmp(flags[0],"path") == 0)
                {
			//if only path
			if(counter == 1)
			{
				//displays environmental variable PATH
				printf("Directory: %s\n", getenv("PATH"));
				exit(getpid());
			}
			//if path +
			else if(strcmp(flags[1], "+") == 0)
			{
				//tries to remove
				strcat(getenv("PATH"), ":");
				strcat(getenv("PATH"), flags[2]);
				setenv("PATH", getenv("PATH"), 1);
				printf("Directory: %s\n", getenv("PATH"));
				exit(getpid());
			}
			//if path -
			else if(strcmp(flags[1], "-") == 0)
			{
				//Tries to remove the path
				string = strstr(getenv("PATH"), flags[2]);
				string--;
				strncpy(string, "", 1);
				setenv("PATH", getenv("PATH"), 1);
				//tries to diplay what I want
				printf("Directory: %s\n", getenv("PATH"));
				exit(getpid());
			}
			//if unknown
			else
			{
				printf("Unknown Argument\n");
				return;
			}
		}
		last = counter++;
		flags[last] = NULL; //sets null terminating character
		execvp(flags[0], flags);//calls the command
		printf("EXECVP failed\n");//if the process could not be called, terminate child
		exit(getpid());
	}
	else
	{
		wait( ( int *) 0);//waits for child to finish
	}
	return;
}

