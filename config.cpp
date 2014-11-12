#include "config.h"

using namespace mpc_lcd;

config_t Config::config()const
{
	return this->config_data;
}

int Config::load_config(const std::string &cfg_file)
{
	INIReader ini(cfg_file);
	if (ini.ParseError() < 0) {
		return 1;
	}
	this->config_data.uid = ini.GetInteger("", "uid", 0);
	this->config_data.ssd3306_interface = ini.Get("", "ssd3306_interface", "");
	this->config_data.ssd3306_i2c_address = ini.Get("", "ssd3306_i2c_address", "");

	return 0;
}

