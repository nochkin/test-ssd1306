#ifndef _MPCLIENT_H
#define _MPCLIENT_H

#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#include <mpd/connection.h>
#include <mpd/status.h>
#include <mpd/list.h>
#include <mpd/player.h>
#include <mpd/queue.h>
#include <mpd/response.h>
#include <mpd/song.h>
#include <mpd/audio_format.h>
#include <mpd/idle.h>
#include <mpd/tag.h>

struct mpd_info_s {
	int volume;
	uint8_t state;
	uint16_t elapsed_time;
};

struct mpd_info_song_s {
	uint8_t kbit_rate;
	uint32_t sample_rate;
	uint8_t channels;
	uint8_t bits;
	std::string title;
	std::string album;
	std::string artist;
};

class MPClient
{
	public:
		struct mpd_connection *my_mpd_conn;

		MPClient();
		MPClient(std::string host, uint16_t);
		~MPClient();
		int connect();
		int disconnect();
		void update_status();
		void print_status();

		int get_info_volume();
		uint8_t get_info_state();
		uint16_t get_info_elapsed_time();
		uint8_t get_info_kbit_rate();
		uint32_t get_info_sample_rate();
		uint8_t get_info_channels();
		uint8_t get_info_bits();
		std::string get_info_title();
		std::string get_info_album();
		std::string get_info_artist();
		std::string get_info_song_tag(mpd_tag_type tag_type);
		bool add_and_play(std::string playfile);
	private:
		std::string mpd_host;
		uint16_t mpd_port;

		struct mpd_status *my_mpd_status;
		struct mpd_song *my_mpd_song;
		const struct mpd_audio_format *mpd_audio_format;

		struct mpd_info_s *mpd_info;
		struct mpd_info_song_s *mpd_info_song;

		bool do_add_and_play(std::string playfile);
		std::string get_song_tag_or_empty(mpd_tag_type tag_type);
};

#endif
