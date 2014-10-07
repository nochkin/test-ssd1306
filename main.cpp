#include <stdio.h>

#include <mpd/status.h>

#include "mpclient.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

int main(int argc, char *argv[])
{
	Adafruit_SSD1306 display;
	MPClient mpc_client;

	if (display.init_i2c() != 0) {
		printf("Failed init OLED\n");
		return 2;
	}

	display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.write('t');
    display.drawCircle(3, 3, 8, WHITE);
	display.display();

	mpc_client.connect();
	mpc_client.loop();

	mpc_client.update_status();
	printf("mpc: volume: %i\n", mpc_client.get_info_volume());
	printf("mpc: state: ");
	switch (mpc_client.get_info_state()) {
		case MPD_STATE_STOP:
			printf("stop\n");
			break;
		case MPD_STATE_PLAY:
			printf("play\n");
			break;
		case MPD_STATE_PAUSE:
			printf("pause\n");
			break;
	}
	printf("mpc: time: %i\n", mpc_client.get_info_elapsed_time());
	printf("mpc: audio: %iHz/%ibit", mpc_client.get_info_sample_rate(), mpc_client.get_info_bits());
	switch(mpc_client.get_info_channels()) {
		case 0:
			printf("\n");
			break;
		case 1:
			printf("/mono\n");
			break;
		case 2:
			printf("/stereo\n");
			break;
		default:
			printf("/multi-channel\n");
			break;
	}
	printf("mpc: title: %s\n", mpc_client.get_info_title().c_str());
	printf("mpc: album: %s\n", mpc_client.get_info_album().c_str());

	return 0;
}

