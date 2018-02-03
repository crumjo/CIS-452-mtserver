#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


void* get_file(void* arg);

//Global data
char fileName[32];
pthread_t threadID[10];
int status;
int fileCount = 0;
int threadUsed[10];
int threadNum;

//^C signal handler
void sig_handler(int sigNum){
	printf("\nFiles processed: %d\n", fileCount);
	exit(0);
} 

int main(){
	int i,j;
	signal(SIGINT, sig_handler);
	for(j = 0; j < 10; j++){
		threadUsed[j] = 0;
	}
	//While loop to keep the program asking for filename
	while(1){
		strcpy(fileName,"");
		printf("Enter a filename: ");
		scanf("%s", fileName);
		
		for(i = 0; i < 10; i++){
			if(threadUsed[i] == 0){
				threadNum = i;
				threadUsed[i] = 1;
				break;
			}
		}
		if((status = pthread_create(&threadID[threadNum], NULL, get_file, NULL)) != 0){
			fprintf(stderr, "Thread create error %d: %s\n", status, strerror(status));
		}
		
	}
	return 0;

}

void* get_file(void* arg){
	int num = threadNum;
	char file[32];
	strcpy(file,fileName);
	int per = rand() % 10;
	int high = rand() % 4;
	
	if(per < 7){
		sleep(1);
	}else{
		sleep(high+7);
	}
	
	printf("\nFile %s was accessed.\n",file);
	printf("Thread: %d\n",num );
	fileCount++;
	threadUsed[num] = 0;
	return NULL;
}
