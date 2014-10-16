#include "config.h"

config_t Config::config()
{
	return this->config_data;
}

int Config::load_config(std::string cfg_file)
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

void Config::set_config(config_t config)
{
	this->config_data = config;
}

