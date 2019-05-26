/**
 * @file
 * @brief
 *
 * @date 17.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <kernel/task.h>
#include "environment.h"

#define ITER 100

int period[3]= { 0, 0, 0 };

int current[3] = { 0, 0, 0 };
int var[ITER][3];


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
	int tid1, tid2, tid3;
	int var_full[ITER * 3];

	tid1 = new_task("Task 1", task, (void *) 0);
	tid2 = new_task("Task 2", task, (void *) 1);
	tid3 = new_task("Task 3", task, (void *) 2);

	task_waitpid(tid1);
	task_waitpid(tid2);
	task_waitpid(tid3);

	for (int i = 0; i < ITER; i++) {
		for (int j = 0; j < 3; j++) {
			var_full[i * 3 + j] = var[i][j];
		}
	}

	output("Process", var_full, ITER * 3);

	return 0;
}
