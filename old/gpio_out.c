#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

/*
 * sudo chown root select_sticker
 * sudo chmod u+s select_sticker
 */

int main(int argc, char **argv) {
	int gpio, val;
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [gpio] [val]\n", argv[0]);
		return 1;
	}

	gpio = strtoul(argv[1], NULL, 0);
	if (gpio < 0 || gpio > 190) {
		fprintf(stderr, "Error: gpio number is out of range (1 .. 190)\n");
		return 1;
	}

	val = strtoul(argv[2], NULL, 0);

	gpio_export(gpio);
	gpio_set_direction(gpio, GPIO_OUT);
	gpio_set_value(gpio, !!val);
	return 0;
}
