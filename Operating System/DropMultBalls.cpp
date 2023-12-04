/*

	This program drops down multiple balls by multiple threads.
	There is a critical region specified by the top and bottom coordinates.

	Mission: 
		[IMPORTANT] Read this code carefully and fully understand.
		Modify the code to ensure that only T balls can enter the critical region at a time.
			hw7_1.c: T = 1
				use a mutex (pthread_mutex_t)
			hw7_2.c: T is specified by the command line argument
				use a counting semaphore (sem_t)

	Hints) 
		1. Pass the top and bottom coordinates of the critical section to the threads through the ThreadParam structure
		2. Declare a mutex or semaphore as a global variable
		3. Initialize the mutex or semaphore
		4. When a ball enters the critical region, wait the mutex or semaphore (entry section)
		4. When a ball leaves the critical region, signal the mutex or semaphore (exit section)
		5. If the thread breaks the loop while the ball is in the critical region, signal the mutex or semaphore to release other threads.
		6. After the threads finishes, destroy the mutex or semaphore.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <pthread.h>
// TO DO: if necessary, include more header files
#include <semaphore.h>
#include <ctype.h>
#include "Console.h"

#define TRUE 1
#define FALSE 0

#define MAX_THREAD 30

typedef struct {
	int thread_idx;
	int no_thread;
	int screen_height;
	int x;
	int delay;

	// TO DO: add fields to pass the vertical coordinates of the critical region
	int crit_top;
	int crit_btm;
} ThreadParam;

int thread_cont = TRUE;			// global variable to control the loop in the thread function

void* ThreadFn(void *vParam);

// TO DO: declare a mutex or semaphore as a global variable
sem_t sem;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	clrscr(); //clear screen 

	int no_thread = 0;
	if(argc > 1)
		no_thread = atoi(argv[1]);

	if(no_thread > MAX_THREAD)
		no_thread = MAX_THREAD;
	else if(no_thread == 0)
		no_thread = 5;

	// TO DO: initialize the mutex or semaphore.
	//		on failure, display an error message and quit
	if(sem_init(&sem, 0, atoi(argv[2]))==-1 || !isdigit(argv[2][0])){
		perror("error sem_init or # of semaphore");
		return -1;
	}
	int screen_width = getWindowWidth(); //get width of current console window
	int screen_height = getWindowHeight() - 3; //get height of current console window
	// the top and bottom coordinates of the critical region
	int critical_top = screen_height / 3;
	int critical_bottom = screen_height * 2 / 3;

	// mark the critical region by horizontal lines
	DrawLine(1, critical_top, screen_width, critical_top, '-');
	DrawLine(1, critical_bottom, screen_width, critical_bottom, '-');
	
	pthread_t tid[MAX_THREAD];
	ThreadParam param[MAX_THREAD];

	for(int i = 0; i < no_thread; i++){
		param[i].thread_idx = i;
		param[i].no_thread = no_thread;
		param[i].screen_height = screen_height;
		param[i].x = screen_width * (i + 1) / (no_thread + 1);
		param[i].delay = rand() % 300;

		// TO DO: add code to store the vertical coordinates of the critical region in param[i]
		param[i].crit_top=critical_top;
		param[i].crit_btm=critical_bottom;

		pthread_create(&tid[i], NULL, ThreadFn, &param[i]);
	}
	PrintXY(1, screen_height + 1, "Press Enter to quit!\n");
	getchar();

	// termainte the threads
	thread_cont = FALSE;

	for(int i = 0; i < no_thread; i++)
		pthread_join(tid[i], NULL);

	// TO DO: destroy mutex or semaphore
	sem_destroy(&sem);

	clrscr();
	gotoxy(1, 1); //move cursor to (x,y) coordinate`
	printf("Bye!\n");

	return 0;
}

void* ThreadFn(void *vParam)
{
	ThreadParam *param = (ThreadParam*)vParam;

	int y = 1;
	int oldy = 1;
	while(thread_cont){
		// TO DO: implement entry section here 
		if(y==param->crit_top)
			sem_wait(&sem);
		
		PrintXY(param->x, oldy, " ");
        PrintXY(param->x, y, "*");

		fflush(stdout);

		// TO DO: implement exit section here 
		if(y==param->crit_btm)
			sem_post(&sem);
		
		oldy = y;
		y++;
		if(y > param->screen_height)
			y = 1;

		gotoxy(1, 1);
		fflush(stdout);
		usleep(param->delay * 2000);
	}

	// TO DO: if current broke loop in the critical region, unlock mutex
	sem_post(&sem);
	return NULL;
}


