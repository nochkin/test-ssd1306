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
	mpc_client.loop();
}

void Monitor::update_status()
{
	mpc_client.update_status();
}

