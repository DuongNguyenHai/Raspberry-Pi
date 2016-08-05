#include <stdio.h>
#include <unistd.h> // for sleep
#include <pthread.h>


#define NUM_THREADS     5

void *Thread1(void *threadid)
{
	while(1){
		printf("thread 1 : hello\n");
		sleep(3);
	}
}

void *Thread2(void *threadid)
{
	while(1){
		printf("thread 2 : hello\n");
		sleep(4);
	}
}

int main ()
{
	pthread_t threads[NUM_THREADS];
	int rc;
	int i = 0;
	
	printf("Creating thead %d, \n",i);

	if ( (rc = pthread_create(&threads[i], NULL, Thread1, NULL)) ){
		printf("Error:unable to create thread, %d\n",rc );
		return 0;
	}

	i++;
	printf("Creating thead %d, \n",i);

	if ( (rc = pthread_create(&threads[i], NULL, Thread2, NULL)) ){
		printf("Error:unable to create thread, %d\n",rc );
		return 0;
	}

	while(1){
		printf("main thread : hello\n");
		sleep(2);
	}

	pthread_exit(NULL);
	return 0;
}