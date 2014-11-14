#include <syslog.h>
#include <mpd/status.h>

#include "monitor.h"

using namespace mpc_lcd;

Monitor::Monitor()
{
	Adafruit_SSD1306 display;
	MPClient_cb mpc_client;
}

int Monitor::setup_display()
{
	if (display.init_i2c() != 0) {
		return 2;
	}

	display.dim(1);
	display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	// display.write('t');
	// display.drawCircle(3, 3, 8, WHITE);
	display.display();

	return 0;
}

int Monitor::setup_mpc()
{
	if (mpc_client.connect() != 0) {
		return 3;
	}

	return 0;
}

void Monitor::watch_loop()
{
	mpc_client.update_status();
	on_player();

	void (Monitor::*playerCB)() = &Monitor::on_player;
	mpc_client.set_callback_player(this, playerCB);

	mpc_client.loop();
}

void Monitor::on_player()
{
	display.clearDisplay();
	display.setTextColor(WHITE);
	display.setTextSize(1);
	display.setTextWrap(true);
	switch (mpc_client.get_info_state()) {
		case MPD_STATE_STOP:
			display.fillRect(0, 0, 6, 6, WHITE);
			break;
		case MPD_STATE_PLAY:
			display.fillTriangle(0, 0, 6, 3, 0, 6, WHITE);
			break;
		case MPD_STATE_PAUSE:
			display.fillRect(0, 0, 2, 6, WHITE);
			display.fillRect(4, 0, 2, 6, WHITE);
			break;
	}

	uint16_t mpc_sample_rate = mpc_client.get_info_sample_rate();
	uint16_t mpc_bits = mpc_client.get_info_bits();
	uint8_t mpc_channels = mpc_client.get_info_channels();
	std::string mpc_title = mpc_client.get_info_title();
	std::string mpc_album = mpc_client.get_info_album();

	display.setCursor(0,10);
	if (mpc_sample_rate > 0) {
		display.printf("%iHz", mpc_sample_rate);
		if (mpc_bits > 0) {
			display.printf("/%ibit", mpc_bits);
			if (mpc_channels == 1) {
				display.print("/mono");
			} else if (mpc_channels == 2) {
				display.print("/stereo");
			} else if (mpc_channels > 2) {
				display.print("/multi-channel");
			}
		}
	}

	display.setCursor(0, 19);
	if (!mpc_title.empty()) {
		display.print(mpc_title.c_str());
		syslog(LOG_DAEMON|LOG_INFO, "title: %s\n", mpc_title.c_str());
		printf("title: %s\n", mpc_title.c_str());
	} else if (!mpc_album.empty()) {
		display.print(mpc_album.c_str());
		syslog(LOG_DAEMON|LOG_INFO, "album: %s\n", mpc_album.c_str());
		printf("album: %s\n", mpc_album.c_str());
	}

	display.display();
}

void Monitor::update_status()
{
	mpc_client.update_status();
}

