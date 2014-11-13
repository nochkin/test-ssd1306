#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>

#include "INIReader.h"

namespace mpc_lcd {
	struct config_t
	{
		std::string ssd1306_interface;
		std::string ssd1306_i2c_address;
		std::string ssd1306_spi_cs;
		uint8_t ssd1306_spi_cs_int;
		int uid;
	};

	class Config
	{
		public:
			static Config& get()
			{
				static Config instance;
				return instance;
			}
			config_t config()const;
			int load_config(const std::string&);
		private:
			Config() {};
			Config(Config const&);
			void operator=(Config const&);

			void tolower(std::string *);

			config_t config_data;
	};
}

#endif
