#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>



/**
 * Header function for a worker thread 
 * on a server. Waits 1 second 80% of the
 * time to simulate memory access and 7-10
 * seconds to simulate disk access.
 * 
 * @param arg a generic argument for a variety of types.
 * @return void* a generic type return.
 */
void* get_file(void* arg);


/* Global/shared data. */
char fileName[32];
pthread_t threadID[10];
int status;
int fileCount = 0;
int threadUsed[10];
int threadNum;


/* ^c signal handler. */
void sig_handler(int sigNum){
	printf("\nFiles processed: %d\n", fileCount);
	exit(0);
} 


/**
 * Main method that takes user input, dispatches
 * a worker thread to retrieve the file, and 
 * then immediately accept new user input.
 */
int main()
{
	int i, j;
	signal(SIGINT, sig_handler);
	for(j = 0; j < 10; j++)
    {
		threadUsed[j] = 0;
	}
	
    /* While loop to keep the program asking for filename. */
	while(1)
    {
        printf("Enter a filename: ");
        fgets(fileName, 32, stdin);
        
        /* Goes to next loop if user does not enter a file name. */
        if(fileName[0] == '\n')
        {
            printf("Blank file name entered. Enter a file name.\n");
            continue;
        }
        
        /* Remove trailing newline. */
        int len = strlen(fileName);
        if (fileName[len - 1] == '\n')
            fileName[len - 1] = '\0';
        
        /* Max number of threads is 10. */
		for(i = 0; i < 10; i++)
        {
			if(threadUsed[i] == 0)
            {
				threadNum = i;
				threadUsed[i] = 1;
				break;
			}
		}
		if((status = pthread_create(&threadID[threadNum], NULL, get_file, NULL)) != 0)
        {
			fprintf(stderr, "Thread create error %d: %s\n", status, strerror(status));
		}
	}
	return 0;
}


/**
 * Function that simulates a worker thread 
 * on a server. Waits 1 second 80% of the
 * time to simulate memory access and 7-10
 * seconds to simulate disk access.
 *
 * @param arg a generic argument for a variety of types.
 * @return void* a generic type return.
 */
void* get_file(void* arg)
{
	int num = threadNum;
	char file[32];
	int per = rand() % 10;
	int high = rand() % 4;
	int sec = 0;

	strcpy(file, fileName);

	if(per < 7){
        sec = 1;
		sleep(sec);
	}
	else
	{
        sec = high + 7;
		sleep(sec);
	}
	
	printf("\nFile %s was accessed by thread %d in %d second(s).\n", 
		file, num, sec);
	fileCount++;
	threadUsed[num] = 0;
	
	return NULL;
}
