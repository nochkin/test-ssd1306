#include "monitor.h"

Monitor::Monitor()
{
	Adafruit_SSD1306 display;
	MPClient mpc_client;
}

int Monitor::setup_display()
{
	if (display.init_i2c() != 0) {
		return 2;
	}

	display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.write('t');
	display.drawCircle(3, 3, 8, WHITE);
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
	void (Monitor::*playerCB)() = &Monitor::on_player;
	mpc_client.set_callback_player(playerCB);

	mpc_client.loop();
}

void Monitor::on_player()
{
	// printf("notify\n");
	display.clearDisplay();
	display.setTextColor(1);
	display.setTextSize(1);
	display.setTextWrap(true);
	switch (mpc_client.get_info_state()) {
		case MPD_STATE_STOP:
			display.fillRect(1, 1, 5, 5, 1);
			break;
		case MPD_STATE_PLAY:
			display.fillTriangle(1, 1, 6, 3, 1, 6, 1);
			break;
		case MPD_STATE_PAUSE:
			display.fillRect(1, 1, 2, 5, 1);
			display.fillRect(4, 1, 2, 5, 1);
			break;
	}

	uint16_t mpc_sample_rate = mpc_client.get_info_sample_rate();
	uint16_t mpc_bits = mpc_client.get_info_bits();
	uint8_t mpc_channels = mpc_client.get_info_channels();
	std::string mpc_title = mpc_client.get_info_title();
	std::string mpc_album = mpc_client.get_info_album();

	display.setCursor(0,6);
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

	display.setCursor(0, 14);
	if (!mpc_title.empty()) {
		display.print(mpc_title.c_str());
	} else if (!mpc_album.empty()) {
		display.print(mpc_album.c_str());
	}
}

void Monitor::update_status()
{
	mpc_client.update_status();
}

