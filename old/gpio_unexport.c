#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

/*
 * sudo chown root select_sticker
 * sudo chmod u+s select_sticker
 */

int main(int argc, char **argv) {
	int gpio, val;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s [gpio]\n", argv[0]);
		return 2;
	}

	gpio = strtoul(argv[1], NULL, 0);
	if (gpio < 0 || gpio > 190) {
		fprintf(stderr, "Error: gpio number is out of range (1 .. 190)\n");
		return 2;
	}

	gpio_unexport(gpio);
	return 0;
}
