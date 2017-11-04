#include "shell.h"
int main()
{
	int input;// for mode selection
	int status = 1;//while loop status
	

	while(status)//run until a mode is called
	{
		printf("Press 1 for interactive mode or 2 for batch mode: ");
		scanf("%d", &input);
		if(input == 1)//run in interactive mode
		{
			interactive();
			status = 0;
		}
		else if(input == 2)//run in batch mode
		{
			batch();
			status = 0;
			
		}
		else//input check
		{
			printf ("Invalid input \n");
		}
	}
	printf("\n Program exiting.\n");
	return 0;
}