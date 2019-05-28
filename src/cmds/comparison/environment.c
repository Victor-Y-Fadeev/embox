/**
 * @file
 * @brief
 *
 * @date 16.05.19
 * @author Victor Y. Fadeev
 */

#include "environment.h"
#include <stdio.h>


void recursive_printf(double value, int precision) {
	if (precision == 0) {
		return;
	}

	value *= 10;
	printf("%d", (int) value);
	recursive_printf(value - (int) value, precision - 1);
}

void double_printf(double value) {
	if (value < 0) {
		value *= -1;
	}

	printf("%d.", (int) value);

	double fractional = value;
	for (int i = 0; i < PRECISION; i++) {
		fractional *= 10;
	}

	if ((int) fractional == 0) {
		recursive_printf(value - (int) value, PRECISION * 2);
	} else {
		recursive_printf(value - (int) value, PRECISION);
	}
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
	double_printf(((double) average) / iter);
	printf(" us\r\n");

	double variance = 0;
	for (int i = 0; i < iter; i++) {
		double temp = var[i] - ((double) average) / iter;
		variance += temp * temp;
	}
	printf("Variance: ");
	double_printf(variance / iter);
	printf("\r\n\r\n");

	for (int i = 0; i < iter; i++) {
		printf("#%d switch - %d microsecond\r\n", i, var[i]);
	}
}

void single(const char *str, const double average) {
	printf("\r\n---%s---\r\n", str);
	printf("Average: ");
	double_printf(average * 1000);
	printf(" ns\r\n\r\n");
}

void error(const char *str) {
	printf("Error: %s\r\n", str);
}
