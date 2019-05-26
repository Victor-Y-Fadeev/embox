/**
 * @file
 * @brief
 *
 * @date 17.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <kernel/sched/sync/mutex.h>
#include <kernel/thread/sync/mutex.h>
#include "environment.h"

#define ITER 1000
#define TASK 2
#define MD(x) do {\
x;\
}while(0);


struct mutex M;
pthread_t th1, th2, th3;

int curent = 0;
int curent2 = 0;
int chek[ITER*TASK];
int chek2[ITER*TASK];
double ave[TASK];
//int ave = -1;


static inline void *task(void *arg) {
    int n = (int)arg; 

    int cur_time = 0;
	int prev_time = 0;

	prev_time = clock();
	for(int i = 0; i < ITER; i++)
	{
		mutex_lock(&M);
            chek[curent] += 1;
            curent++;
        mutex_unlock(&M);
	}
	cur_time = clock();

	ave[n] = cur_time - prev_time;

	prev_time = clock();
	for(int i = 0; i < ITER; i++)
	{
		chek2[curent2] += 1;
        curent2++;
	}
	cur_time = clock();

	ave[n] = (ave[n] - cur_time + prev_time)/ITER; 
   return NULL; 
}

int main(int argc, char **argv) { 

    for (int i = 0; i < ITER*2; i++) {
        chek[i] = -1;
    }

	pthread_attr_t attr;
    pthread_attr_init(&attr);

    mutex_init(&M);
    pthread_create(&th1, &attr, task, (void *)0);
    pthread_create(&th2, &attr, task, (void *)1);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    
    for (int i = 0; i < ITER*TASK; i+=10) {
        if (chek[i] != 0) {
            error("Semaphore"); 
        }
    }
    ave[0] =  (ave[0] + ave[1])/2;
    single("Semaphore",ave[0]);

    return 0;
}

