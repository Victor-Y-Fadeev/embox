/**
 * @file
 * @brief
 *
 * @date 27.05.19
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

pthread_t th1, th2, th3;
struct mutex m;

int current[2] = { 0, 0 };
int check[2][ITER * TASK];
double average[TASK];


static void *task(void *arg) {
	int n = (int) arg;
	int cur_time, prev_time;

	prev_time = clock();
	for (int i = 0; i < ITER; i++) {
		mutex_lock(&m);
		check[0][current[0]]++;
		current[0]++;
		mutex_unlock(&m);
	}
	cur_time = clock();

	average[n] = cur_time - prev_time;

	prev_time = clock();
	for (int i = 0; i < ITER; i++) {
		check[1][current[1]]++;
		current[1]++;
	}
	cur_time = clock();

	average[n] = (average[n] - cur_time + prev_time) / ITER;

	return NULL; 
}

int main(int argc, char **argv) {
	for (int i = 0; i < ITER * TASK; i++) {
		check[0][i] = -1;
	}

	mutex_init(&m);

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&th1, &attr, task, (void *) 0);
	pthread_create(&th2, &attr, task, (void *) 1);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	for (int i = 0; i < ITER * TASK; i++) {
		if (check[0][i] != 0) {
			error("Semaphore fail"); 
		}
	}

	double result = 0;
	for (int i = 0; i < TASK; i++) {
		result += average[i];
	}

	single("Semaphore", result / TASK);
	return 0;
}
