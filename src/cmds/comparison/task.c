/**
 * @file
 * @brief
 *
 * @date 26.05.19
 * @author Victor Y. Fadeev
 */

#include <stdio.h>
#include <pthread.h>
#include "environment.h"

pthread_t th;
pthread_attr_t attr;


static void *task(void *arg) {
	return NULL;
}

int main(int argc, char **argv) {
	pthread_attr_init(&attr);
	pthread_create(&th, &attr, task, NULL);

	pthread_join(th, NULL);
	return 0;
}
