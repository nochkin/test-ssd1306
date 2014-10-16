#include <stdio.h>
#include <getopt.h>

#include "config.h"
#include "monitor.h"

int main(int argc, char *argv[])
{
	int err;
	Monitor mon;
	int c;
	std::string cfg_file = "ssd3306-mpc.cfg";

	while ((c = getopt(argc, argv, "c:")) != EOF) {
		switch (c) {
			case 'c':
				cfg_file = optarg;
				break;
			default:
				return 1;
				break;
		}
	}

	err = mon.setup_display();
	if (err != 0) {
		printf("Failed init OLED\n");
		return 101;
	}

	err = mon.setup_mpc();
	if (err != 0) {
		printf("Failed init mpc\n");
		return 102;
	}

	mon.watch_loop();

	return 0;
}

