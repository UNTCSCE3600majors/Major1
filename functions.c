#include "shell.h"
//Runs the shell in interactive mode.
void interactive()
{
	char *read = NULL; // string from input
	char *semicolon = NULL;//strtok var
	int exitstatus = 1;//checks if the user has told the program to exit
	read = malloc(sizeof(char)*100);
	int counter = 0;
	int stdin_cpy;
	int stdout_cpy;
	
	dup2(0,stdin_cpy);
	dup2(1,stdout_cpy);
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
				dup2(stdin_cpy,0);
				dup2(stdout_cpy,1);
				counter++;
			}
			semicolon = strtok(NULL,"[; ]\n");
			
		}

		while(counter)//waits for all children in the line to terminate
		{
			wait(NULL);
			counter--;
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
	int stdin_cpy;
	int stdout_cpy;
	
	dup2(0,stdin_cpy);
	dup2(1,stdout_cpy);
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
			dup2(stdin_cpy,0);
			dup2(stdout_cpy,1);
			semicolon = strtok(NULL,"[; ]\n");
		}

	}
	fclose(fstream); // close file
	return;
}
//call for each line of commands. Possibly replace spaces with -'s
void execute(char *command)
{

	char newDirectory[255];
	getcwd(newDirectory, 255);
	int counter = 0;//checks the number of args passed to the function
	char *flags[100];//
	char *split = NULL;//strtok var
	char *redirect = NULL;//strtok var
	char *append = NULL;
	char *temp = NULL, *temp2 = NULL;
	int fd[2];
	int stdin_cpy;
	int stdout_cpy;
	int last;//holds the last arg position
	
	dup2(0,stdin_cpy);
	dup2(1,stdout_cpy);
	//strcpy(append,"<<");

	printf("\nExecuting %s \n", command);
	
	if(fork() == 0)//creates a child process
	{
		if((redirect = strchr(command, '<')) && (redirect = strchr(command, '>')))
		{
			temp = strchr(redirect,'<');
			while (strlen(temp) < 2)
			{
				temp = strtok(redirect," <");
			}
			fd[0] = open(temp,O_RDONLY);//opens the inputfile
			temp2 = strchr(redirect,'>');
			while (strlen(temp2) < 2)
			{
				temp2 = strtok(redirect," >");
			}
			fd[1] = open(temp2, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);//opens the outputfile
			//printf("Opened filename %s \n",temp);
			dup2(fd[0],0);//makes stdin read from the file
			dup2(fd[1],1);//makes stdin write to the file
			close(fd[0]);
			close(fd[1]);
	
			redirect = strtok(command, "<");
			//printf("%s\n", redirect);
			execute(redirect);
			dup2(stdin_cpy,0);
			dup2(stdout_cpy,1);
			exit(getpid());
		}
		else if(redirect = strchr(command,'<')) // checks for input operator
		{
			temp = strtok(redirect," <");
			while (strlen(temp) < 2)
			{
				temp = strtok(NULL," <");
			}
			fd[0] = open(temp,O_RDONLY);//opens the inputfile
			//printf("Opened filename %s \n",temp);
			dup2(fd[0],0);//makes stdin read from the file
			close(fd[0]);
	
			redirect = strtok(command, "<");
			//printf("%s\n", redirect);
			execute(redirect);
			dup2(stdin_cpy,0);
			exit(getpid());
			
		}
		/*else if (redirect = strtok(command,">>")) // searches for append operator
		{
			temp = strtok(redirect, ">>");
			while(strlen(temp) < 2)
			{
					temp = strtok(NULL, ">>");
			}
			fd[1] = open(temp, O_WRONLY | O_APPEND | O_TRUNC, S_IRUSR | S_IWUSR); //opens and sets append
			dup2(fd[1], 1);
			close(fd[1]);
			
			redirect = strtok(command, ">>");
			execute(redirect);
			dup2(stdout_cpy, 1);
			exit(getpid());
		}*/ 
		else if(redirect = strchr(command,'>')) // checks for output operator
		{
			temp = strtok(redirect," >");
			while (strlen(temp) < 2)
			{
				temp = strtok(NULL," >");
			}
			fd[1] = open(temp, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);//opens the outputfile
			//printf("Opened filename %s \n",temp);
			dup2(fd[1],1);//makes stdin read from the file
			close(fd[1]);
	
			redirect = strtok(command, ">");
			//printf("%s\n", redirect);
			execute(redirect);
			dup2(stdout_cpy,1);
			exit(getpid());
		}
		else
		{
			split = strtok(command, " "); // searches the commands for flags
			while (split != NULL)
			{
				flags[counter]= split;
				counter++;
				split = strtok(NULL, " ");//checks for remaining spaces.

			}
			if(strcmp(flags[0],"cd") == 0)
			{
				chdir(flags[1]);
				return;
			}

			last = counter++;
			flags[last] = NULL; //sets null terminating character
			execvp(flags[0], flags);//calls the command
			printf("EXECVP failed\n");//if the process could not be called, terminate child
			exit(getpid());//terminate the process if it failed
			free(flags);
			free(split);
			free(redirect);
			//dup2(stdin_cpy,0);
		
		}
		
	}
	return;
}
