#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

/*
 * sudo chown root select_sticker
 * sudo chmod u+s select_sticker
 */

int main(int argc, char **argv) {
	int sticker;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s 1/2/3/4\n", argv[0]);
		return 1;
	}

	sticker = strtoul(argv[1], NULL, 0);
	if (sticker != 1 && sticker != 2 && sticker != 3 && sticker != 4) {
		fprintf(stderr, "Error: Sticker must be 1, 2, 3, or 4\n");
		return 1;
	}

	sticker--;
	gpio_export(55);
	gpio_export(56);
	gpio_set_direction(55, GPIO_OUT);
	gpio_set_direction(56, GPIO_OUT);
	gpio_set_value(55, !!(sticker & 1));
	gpio_set_value(56, !!(sticker & 2));
	return 0;
}
