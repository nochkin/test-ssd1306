#ifndef _MPCLIENT_H
#define _MPCLIENT_H

#include <unistd.h>
#include <stdint.h>
#include <string>

#include <mpd/tag.h>

struct mpd_connection;

namespace mpc_lcd {
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

			MPClient(const std::string &host="", uint16_t port=0);
			~MPClient();
			int connect();
			int disconnect();
			void update_status();
			void print_status()const;

			int get_info_volume()const;
			uint8_t get_info_state()const;
			uint16_t get_info_elapsed_time()const;
			uint8_t get_info_kbit_rate()const;
			uint32_t get_info_sample_rate()const;
			uint8_t get_info_channels()const;
			uint8_t get_info_bits()const;
			std::string get_info_title()const;
			std::string get_info_album()const;
			std::string get_info_artist()const;
			bool add_and_play(const std::string &playfile);
		private:
			std::string mpd_host;
			uint16_t mpd_port;

			struct mpd_status *my_mpd_status;
			struct mpd_song *my_mpd_song;
			const struct mpd_audio_format *mpd_audio_format;

			struct mpd_info_s *mpd_info;
			struct mpd_info_song_s *mpd_info_song;

			bool do_add_and_play(const std::string &playfile);
			std::string get_song_tag_or_empty(mpd_tag_type tag_type)const;
	};
}

#endif
