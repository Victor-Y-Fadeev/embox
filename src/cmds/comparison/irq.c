/**
 * @file
 * @brief
 *
 * @date 26.05.19
 * @author Victor Y. Fadeev
 */

#include <stdio.h>
#include <kernel/task.h>
#include <unistd.h>
#include <pthread.h>
#include <util/err.h>
#include <time.h>
#include <kernel/irq.h>
#include "environment.h"

#define ITER 1000000
#define TEST_IRQ_NR 10

pthread_t th;

static irq_return_t test_isr(unsigned int irq_nr, void *dev_id) {
	//irqctrl_clear(irq_nr);
	return IRQ_HANDLED;
}

static void *task(void *arg) {
	int cur_time = 0;
	int prev_time = 0;
	int ave = 0.0;

	prev_time = clock();
	for(int i=0; i < ITER; i++)
	{
		irqctrl_force(TEST_IRQ_NR); 
	}
	cur_time = clock();

	ave = cur_time - prev_time;

	prev_time = clock();
	for(int i=0; i < ITER; i++)
	{
		
	}
	cur_time = clock();

	ave = (ave - cur_time + prev_time);
     
    printf("ave = %d\n", ave);
	//single("irq", ave);

	return NULL;
}

int main(int argc, char **argv) {

	irq_attach(TEST_IRQ_NR, test_isr, 0, (void *) '0', "test_irq");


	pthread_create(&th, NULL, task, NULL);

	pthread_join(th, NULL);

	irq_detach(TEST_IRQ_NR, (void *) '0');
	return 0;
}
