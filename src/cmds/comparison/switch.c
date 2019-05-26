/**
 * @file
 * @brief
 *
 * @date 27.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <pthread.h>
#include "environment.h"

#define ITER 100

pthread_t th[ITER];


static void *task(void *arg) {
	return NULL;
}

int main(int argc, char **argv) {
	int cur_time, prev_time;
	double average;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	prev_time = clock();
	for (int i = 0; i < ITER; i++) {
		pthread_create(&th[i], &attr, task, NULL);
	}
	for (int i = 0; i < ITER; i++) {
		pthread_join(th[i], NULL);
	}
	cur_time = clock();

	average = cur_time - prev_time;

	prev_time = clock();
	for (int i = 0; i < ITER; i++);
	for (int i = 0; i < ITER; i++);
	cur_time = clock();

	average = (average - cur_time + prev_time) / ITER;
	single("Switching test", average);

	return 0;
}
