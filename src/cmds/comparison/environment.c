/**
 * @file
 * @brief
 *
 * @date 16.05.19
 * @author Victor Y. Fadeev
 */

#include "environment.h"
#include <stdio.h>


void two_points(const double value) {
	printf("%d.", (int) value);

	if ((int) (value * 100) % 100 < 10) {
		printf("0");
	}

	printf("%d", (int) (value * 100) % 100);
}

void output(const char *str, int *var, int iter) {
	var = &var[1];
	iter--;

	printf("\r\n---%s---\r\n", str);
	printf("Iterations: %d\r\n", iter);

	int average = 0;
	for (int i = 0; i < iter; i++) {
		average += var[i];
	}
	printf("Average: ");
	two_points(((double) average) / iter);
	printf(" us\r\n");

	double variance = 0;
	for (int i = 0; i < iter; i++) {
		double temp = var[i] - ((double) average) / iter;
		variance += temp * temp;
	}
	printf("Variance: ");
	two_points(variance / iter);
	printf("\r\n\r\n");

	for (int i = 0; i < iter; i++) {
		printf("#%d switch - %d microsecond\r\n", i, var[i]);
	}
}

void single(const char *str, const double average) {
	printf("\r\n---%s---\r\n", str);
	printf("Average: ");
	two_points(average);
	printf(" us\r\n\r\n");
}

void error(const char *str) {
	printf("Error: %s\r\n", str);
}
