#include <stdio.h>

#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "bcm2835.h"

int main(int argc, char *argv[])
{
	Adafruit_SSD1306 display;
	if (display.init_i2c() != 0) {
		printf("Failed init OLED\n");
		return 2;
	}

	display.clearDisplay();

	display.drawPixel(10, 10, WHITE);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.write('t');
    display.drawCircle(3, 3, 8, WHITE);
	display.display();

	return 0;
}

