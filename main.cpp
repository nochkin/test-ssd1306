#include <stdio.h>

#include "monitor.h"

int main(int argc, char *argv[])
{
	int err;

	Monitor mon;

	err = mon.setup_display();
	if (err != 0) {
		printf("Failed init OLED\n");
		return err;
	}

	err = mon.setup_mpc();
	if (err != 0) {
		printf("Failed init mpc\n");
		return err;
	}

	mon.watch_loop();

	return 0;
}

