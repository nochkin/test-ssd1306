#include <mpd/idle.h>

#include "mpclient_cb.h"

using namespace mpc_lcd;

MPClient_cb::MPClient_cb() : MPClient()
{
	on_player_class = NULL;
	on_player_cb = NULL;
}

void MPClient_cb::set_callback_player(Monitor *mon, void (Monitor::*callback_func)())
{
	on_player_class = mon;
	on_player_cb = callback_func;
}

void MPClient_cb::loop()
{
	while(1) {
		mpd_idle idle = mpd_run_idle(my_mpd_conn);
		// if (idle == 0) continue;
		if ((idle & MPD_IDLE_PLAYER) > 0) {
			update_status();
			// print_status();
			if ((on_player_class != NULL) && (on_player_cb != NULL)) {
				(*on_player_class.*on_player_cb)();
			}
		}
		// printf("idle: %i\n", idle);
		if (idle == 0) {
			this->disconnect();
			usleep(10000000);
			this->connect();
		}
		usleep(100000);
	}
}

