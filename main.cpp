#include <stdio.h>
#include <getopt.h>
#include <syslog.h>

#include "config.h"
#include "monitor.h"

int usage(char *exec_name)
{
	printf("Usage:\n");
	printf("%s [-c mpc-lcd.cfg]\n", exec_name);
	return 2;
}

int main(int argc, char *argv[])
{
	int err;
	Monitor mon;
	int c;
	std::string cfg_file = "mpc-lcd.cfg";

	while ((c = getopt(argc, argv, "c:h")) != EOF) {
		switch (c) {
			case 'c':
				cfg_file = optarg;
				break;
			case 'h':
				return usage(argv[0]);
				break;
			default:
				return 1;
				break;
		}
	}

	err = Config::get().load_config(cfg_file);
	if (err) {
		fprintf(stderr, "Failed to load %s\n", cfg_file.c_str());
		syslog(LOG_DAEMON|LOG_ERR, "Failed to load config\n");
		return 101;
	}

	err = mon.setup_display();
	if (err != 0) {
		printf("Failed init OLED\n");
		return 101;
	}

	int uid = Config::get().config().uid;
	printf("uid: %i\n", uid);
	if (uid > 0) {
		setuid(uid);
	}

	err = mon.setup_mpc();
	if (err != 0) {
		printf("Failed init mpc\n");
		return 102;
	}

	mon.watch_loop();

	return 0;
}

