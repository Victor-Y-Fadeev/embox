/**
 * @file
 * @brief
 *
 * @date 17.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <pthread.h>
#include "environment.h"

#define ITER 100

int period[3]= { 0, 0, 0 };

int current[3] = { 0, 0, 0 };
int var[ITER][3];

pthread_t th, th2, th3;


static void *task(void *arg) {
	int n = (int) arg;
	int first, second, cur_time;

	while (current[n] < ITER) {
		first = period[(n + 1) % 3];
		second = period[(n + 2) % 3];
		cur_time = clock();

		if (first != 0 && second != 0) {
			if (first < second) {
				var[current[n]][n] = cur_time - first;
			} else {
				var[current[n]][n] = cur_time - second;
			}
			current[n]++;
		}
		period[n] = clock();
	}

 	return NULL; 
}

int main(int argc, char **argv) { 
	int var_full[ITER * 3];

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&th, &attr, task, (void *) 0);
	pthread_create(&th2, &attr, task, (void *) 1);
	pthread_create(&th3, &attr, task, (void *) 2);

	pthread_join(th, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);

	for (int i = 0; i < ITER; i++) {
		for (int j = 0; j < 3; j++) {
			var_full[i * 3 + j] = var[i][j];
		}
	}

	output("Thread", var_full, ITER * 3);

	return 0;
}
