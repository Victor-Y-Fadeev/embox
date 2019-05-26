/**
 * @file
 * @brief
 *
 * @date 26.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <pthread.h>
#include <kernel/irq.h>
#include "environment.h"

#define ITER 1000000
#define IRQ_NR 10

pthread_t th;


static irq_return_t isr(unsigned int irq_nr, void *dev_id) {
	//irqctrl_clear(irq_nr);
	return IRQ_HANDLED;
}

static void *task(void *arg) {
	int cur_time, prev_time;
	double average;

	prev_time = clock();
	for (int i = 0; i < ITER; i++) {
		irqctrl_force(IRQ_NR); 
	}
	cur_time = clock();

	average = cur_time - prev_time;

	prev_time = clock();
	for (int i = 0; i < ITER; i++);
	cur_time = clock();

	average = (average - cur_time + prev_time) / ITER;
	single("IRQ", average);

	return NULL;
}

int main(int argc, char **argv) {
	irq_attach(IRQ_NR, isr, 0, (void *) '0', "irq");

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&th, &attr, task, NULL);

	pthread_join(th, NULL);
	irq_detach(IRQ_NR, (void *) '0');
	return 0;
}
