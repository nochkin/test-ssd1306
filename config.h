#ifndef _CONFIG_H
#define _CONFIG_H

#include "INIReader.h"

namespace mpc_lcd {
	struct config_t
	{
		std::string ssd3306_interface;
		std::string ssd3306_i2c_address;
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
			config_t config();
			int load_config(std::string);
			void set_config(config_t);
		private:
			Config() {};
			Config(Config const&);
			void operator=(Config const&);

			config_t config_data;
	};
}

#endif
