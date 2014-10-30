#ifndef _MONITOR_H
#define _MONITOR_H

#include "mpclient_cb.h"
#include "Adafruit_SSD1306.h"

class Monitor
{
	public:
		Monitor();
		int setup_display();
		int setup_mpc();
		void on_player();
		void watch_loop();
	private:
		Adafruit_SSD1306 display;
		MPClient_cb mpc_client;

		void update_status();
};

#endif
