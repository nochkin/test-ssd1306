/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#ifdef ARDUINO
#if ARDUINO >= 100
 #include "Arduino.h"
 #define WIRE_WRITE Wire.write
#else
 #include "WProgram.h"
  #define WIRE_WRITE Wire.send
#endif
#else
#include <stdint.h>
#include <bcm2835.h>
#endif

#ifdef __SAM3X8E__
 typedef volatile RwReg PortReg;
 typedef uint32_t PortMask;
#else
  typedef volatile uint8_t PortReg;
  typedef uint8_t PortMask;
#endif

#ifdef ARDUINO
#include <SPI.h>
#endif

#include <Adafruit_GFX.h>

// Configuration pins
#define OLED_SPI_RESET RPI_V2_GPIO_P1_22 /* GPIO 25 pin 22  */
#define OLED_SPI_DC    RPI_V2_GPIO_P1_18 /* GPIO 24 pin 18  */
#define OLED_SPI_CS0   BCM2835_SPI_CS0   /* Chip Select CE0 */
#define OLED_SPI_CS1   BCM2835_SPI_CS1   /* Chip Select CE1 */
#define OLED_SPI_CS    BCM2835_SPI_CS1   /* Default Chip Select */
#define OLED_I2C_RESET RPI_V2_GPIO_P1_22 /* GPIO 25 pin 12  */

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1306_I2C_ADDRESS   0x3C	// 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

/*=========================================================================
    SSD1306 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1306_128_64  128x64 pixel display

    SSD1306_128_32  128x32 pixel display

    SSD1306_96_16

    -----------------------------------------------------------------------*/
   #define SSD1306_128_64
//   #define SSD1306_128_32
//   #define SSD1306_96_16
/*=========================================================================*/

#if defined SSD1306_128_64 && defined SSD1306_128_32
  #error "Only one SSD1306 display can be specified at once in SSD1306.h"
#endif
#if !defined SSD1306_128_64 && !defined SSD1306_128_32 && !defined SSD1306_96_16
  #error "At least one SSD1306 display must be specified in SSD1306.h"
#endif

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH                  96
  #define SSD1306_LCDHEIGHT                 16
#endif

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

enum interface_t {
	IF_SPI,
	IF_I2C,
	IF_NOT_SET
};

class Adafruit_SSD1306 : public Adafruit_GFX {
	public:
		Adafruit_SSD1306();

		// SPI
		int init_spi(int8_t DC = OLED_SPI_DC, int8_t RST = OLED_SPI_RESET, int8_t CS = OLED_SPI_CS);
		// I2C
		int init_i2c(int8_t RST = OLED_I2C_RESET, uint8_t ADDR = SSD1306_I2C_ADDRESS);

		interface_t get_interface();
		void ssd1306_command_old(uint8_t c);
		void ssd1306_command(uint8_t c);
		void ssd1306_command(uint8_t c0, uint8_t c1);
		void ssd1306_command(uint8_t c0, uint8_t c1, uint8_t c2);
		void ssd1306_data(uint8_t c);

		void clearDisplay(void);
		void invertDisplay(boolean i);
		void display();

		void startscrollright(uint8_t start, uint8_t stop);
		void startscrollleft(uint8_t start, uint8_t stop);

		void startscrolldiagright(uint8_t start, uint8_t stop);
		void startscrolldiagleft(uint8_t start, uint8_t stop);
		void stopscroll(void);

		void dim(boolean contrast);

		void drawPixel(int16_t x, int16_t y, uint16_t color);

		virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

	private:
		int8_t _i2caddr, _vccstate, sid, sclk, dc, rst, cs;
		uint16_t _width, _height;
		interface_t _if_type;
		void begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, bool reset=true);
		boolean isSPI(void);
		boolean isI2C(void);
		void fastSPIwrite(uint8_t c);
		void fastSPIwrite(char* tbuf, uint32_t len);
		void fastI2Cwrite(uint8_t c);
		void fastI2Cwrite(char* tbuf, uint32_t len);
		void slowSPIwrite(uint8_t c);

		boolean hwSPI;
		PortReg *mosiport, *clkport, *csport, *dcport;
		PortMask mosipinmask, clkpinmask, cspinmask, dcpinmask;

		inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
		inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));
};
