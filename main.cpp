#include <stdio.h>

#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "bcm2835.h"

// Configuration pins
#define OLED_SPI_RESET RPI_V2_GPIO_P1_22 /* GPIO 25 pin 22  */
#define OLED_SPI_DC    RPI_V2_GPIO_P1_18 /* GPIO 24 pin 18  */
#define OLED_SPI_CS0   BCM2835_SPI_CS0   /* Chip Select CE0 */
#define OLED_SPI_CS1   BCM2835_SPI_CS1   /* Chip Select CE1 */
#define OLED_SPI_CS    BCM2835_SPI_CS1   /* Default Chip Select */
#define OLED_I2C_RESET RPI_V2_GPIO_P1_22 /* GPIO 25 pin 12  */

Adafruit_SSD1306 display(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, 2);

int main(int argc, char *argv[])
{
	display.begin();
	display.clearDisplay();

	return 0;
}

