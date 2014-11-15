#include <algorithm>
#include "bcm2835.h"

#include "config.h"

using namespace mpc_lcd;

config_t Config::config()const
{
	return config_data;
}

int Config::load_config(const std::string &cfg_file)
{
	INIReader ini(cfg_file);
	if (ini.ParseError() < 0) {
		return 1;
	}
	config_data.uid = ini.GetInteger("", "uid", 0);
	config_data.ssd1306_interface = ini.Get("", "ssd1306_interface", "");
	config_data.ssd1306_spi_cs = ini.Get("", "ssd1306_spi_cs", "cs0");
	config_data.ssd1306_i2c_address = ini.Get("", "ssd1306_i2c_address", "");

	this->tolower(config_data.ssd1306_spi_cs);

	config_data.ssd1306_spi_cs_int = BCM2835_SPI_CS0;
	if (config_data.ssd1306_spi_cs == "cs0") {
		config_data.ssd1306_spi_cs_int = BCM2835_SPI_CS0;
	} else if (config_data.ssd1306_spi_cs == "cs1") {
		config_data.ssd1306_spi_cs_int = BCM2835_SPI_CS1;
	} else {
		return 1;
	}

	return 0;
}

void Config::tolower(std::string &input)
{
	std::transform(input.begin(), input.end(), input.begin(), ::tolower);
}
