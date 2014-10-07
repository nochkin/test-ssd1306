#ifndef _MPCLIENT_H
#define _MPCLIENT_H

#include <string>
#include <stdint.h>

struct mpd_info_s {
	int volume;
	uint8_t state;
	uint16_t elapsed_time;
	uint8_t kbit_rate;
	std::string title;
};

class MPClient
{
	public:
		MPClient(char *host, uint16_t);
		bool connect();
		void update_status();

		int get_info_volume();
		uint8_t get_info_state();
		uint16_t get_info_elapsed_time();
		uint8_t get_info_kbit_rate();
		std::string get_info_title();
	private:
		char *mpd_host;
		uint16_t mpd_port;

		struct mpd_connection *mpd_conn;
		struct mpd_status *mpd_status;
		struct mpd_song *mpd_song;
		const struct mpd_audio_format *mpd_audio_format;

		struct mpd_info_s *mpd_info;
};

#endif
