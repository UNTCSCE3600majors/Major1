#include "shell.h"

int main(int argc, char *argv[])
{
	int input;// for mode selection
	int j;
	char *filename;

	while(1)//run until a mode is called
	{
		if(argc == 1)//run in interactive mode
		{
			interactive();
	
			break;
		}
		else if(argc >= 2)//run in batch mode
		{
			for(j=1; j <= argc; j++)
			{
				filename = argv[j];
				batch(filename);
			}
			break;
		}
		else//input check
		{
			printf ("Invalid input \n");
			break;
		}
		printf("Exiting program\n");
	}
	return 0;
}
