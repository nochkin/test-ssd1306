#include <stdio.h>
#include <getopt.h>
#include <syslog.h>

#include "config.h"
#include "monitor.h"

using namespace mpc_lcd;

int usage(char *exec_name)
{
	printf("Usage:\n");
	printf("%s [-c mpc-lcd.cfg]\n", exec_name);
	return 2;
}

int main(int argc, char *argv[])
{
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

	int err = Config::get().load_config(cfg_file);
	if (err) {
		fprintf(stderr, "Failed to load %s\n", cfg_file.c_str());
		syslog(LOG_DAEMON|LOG_ERR, "Failed to load config\n");
		return 101;
	}

	Monitor mon;
	err = mon.setup_display();
	if (err != 0) {
		fprintf(stderr, "Failed init OLED\n");
		syslog(LOG_DAEMON|LOG_ERR, "Failed init OLED\n");
		return 102;
	}

	int uid = Config::get().config().uid;
	if (uid > 0) {
		printf("Using uid: %i\n", uid);
		syslog(LOG_DAEMON|LOG_INFO, "Using uid: %i\n", uid);
		setuid(uid);
	}

	err = mon.setup_mpc();
	if (err != 0) {
		fprintf(stderr, "Failed init mpc\n");
		syslog(LOG_DAEMON|LOG_ERR, "Failed init mpc\n");
		return 103;
	}

	mon.watch_loop();

	return 0;
}

