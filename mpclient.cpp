#include "mpclient.h"

MPClient::MPClient()
{
	mpd_host = "";
	mpd_port = 0;

	my_mpd_conn = NULL;
	mpd_info = new mpd_info_s;
	mpd_info_song = new mpd_info_song_s;
}

MPClient::MPClient(std::string host, uint16_t port)
{
	mpd_host = host;
	mpd_port = port;

	my_mpd_conn = NULL;
	mpd_info = new mpd_info_s;
	mpd_info_song = new mpd_info_song_s;
}

MPClient::~MPClient()
{
	if (my_mpd_conn) mpd_connection_free(my_mpd_conn);
	delete mpd_info_song;
	delete mpd_info;
}

int MPClient::connect()
{
	my_mpd_conn = mpd_connection_new(mpd_host.empty() ? 0 : mpd_host.c_str(), mpd_port, 30000);
	if (mpd_connection_get_error(my_mpd_conn) != MPD_ERROR_SUCCESS) {
		return 1;
	}
	return 0;
}

int MPClient::disconnect()
{
	if (my_mpd_conn) {
	       mpd_connection_free(my_mpd_conn);
	       return 0;
	}
	return 1;
}

void MPClient::update_status()
{
	mpd_info_song->title = "";

	mpd_command_list_begin(my_mpd_conn, true);
	mpd_send_status(my_mpd_conn);
	mpd_send_current_song(my_mpd_conn);
	mpd_command_list_end(my_mpd_conn);
	my_mpd_status = mpd_recv_status(my_mpd_conn);
	if (my_mpd_status == 0) { // error
	} else {
		mpd_info->volume = mpd_status_get_volume(my_mpd_status);
		mpd_info->state = mpd_status_get_state(my_mpd_status);
		if (mpd_info->state == MPD_STATE_PLAY || mpd_info->state == MPD_STATE_PAUSE) {
			mpd_info->elapsed_time = mpd_status_get_elapsed_time(my_mpd_status);
			mpd_info_song->kbit_rate = mpd_status_get_kbit_rate(my_mpd_status);
		}
		mpd_audio_format = mpd_status_get_audio_format(my_mpd_status);
		if (mpd_audio_format != NULL) {
			mpd_info_song->sample_rate = mpd_audio_format->sample_rate;
			mpd_info_song->channels = mpd_audio_format->channels;
			mpd_info_song->bits = mpd_audio_format->bits;
		}

		mpd_status_free(my_mpd_status);

		if (mpd_connection_get_error(my_mpd_conn) != MPD_ERROR_SUCCESS) {
			// errror
		} else {
			mpd_response_next(my_mpd_conn);
			my_mpd_song = mpd_recv_song(my_mpd_conn);
			if (my_mpd_song != NULL) {
				mpd_info_song->title = get_song_tag_or_empty(MPD_TAG_TITLE);
				mpd_info_song->album = get_song_tag_or_empty(MPD_TAG_ALBUM);
				mpd_info_song->artist = get_song_tag_or_empty(MPD_TAG_ARTIST);
			}
		}
	}
	mpd_response_finish(my_mpd_conn);
}

bool MPClient::add_and_play(std::string playfile)
{
	bool status;
	uint8_t tries = 3;

	while (tries > 0) {
		status = do_add_and_play(playfile);
		if (status) break;
		disconnect();
		connect();
		tries -= 1;
	}

	return status;
}

bool MPClient::do_add_and_play(std::string playfile)
{
	bool status;
	while (1) {
		status = mpd_run_clear(my_mpd_conn);
		if (mpd_connection_get_error(my_mpd_conn) != MPD_ERROR_SUCCESS) {
			printf("error: %s\n", mpd_connection_get_error_message(my_mpd_conn));
			break;
		}
		status = mpd_run_add(my_mpd_conn, playfile.c_str());
		if (mpd_connection_get_error(my_mpd_conn) != MPD_ERROR_SUCCESS) {
			printf("error: %s\n", mpd_connection_get_error_message(my_mpd_conn));
			break;
		}
		status = mpd_run_play(my_mpd_conn);
		if (mpd_connection_get_error(my_mpd_conn) != MPD_ERROR_SUCCESS) {
			printf("error: %s\n", mpd_connection_get_error_message(my_mpd_conn));
			break;
		}
		break;
	}

	return status;
}

void MPClient::print_status()
{
	printf("mpc: volume: %i\n", get_info_volume());
	printf("mpc: state: ");
	switch (get_info_state()) {
		case MPD_STATE_STOP:
			printf("stop\n");
			break;
		case MPD_STATE_PLAY:
			printf("play\n");
			break;
		case MPD_STATE_PAUSE:
			printf("pause\n");
			break;
	}
	printf("mpc: time: %i\n", get_info_elapsed_time());
	printf("mpc: audio: %iHz/%ibit", get_info_sample_rate(), get_info_bits());
	switch(get_info_channels()) {
		case 0:
			printf("\n");
			break;
		case 1:
			printf("/mono\n");
			break;
		case 2:
			printf("/stereo\n");
			break;
		default:
			printf("/multi-channel\n");
			break;
	}
	printf("mpc: title: %s\n", get_info_title().c_str());
	printf("mpc: album: %s\n", get_info_album().c_str());
}

std::string MPClient::get_song_tag_or_empty(mpd_tag_type tag_type)
{
	const char *tag_value = mpd_song_get_tag(my_mpd_song, tag_type, 0);
	std::string tag_value_str = tag_value == NULL ? "" : std::string(tag_value);
	return tag_value_str;
}

int MPClient::get_info_volume()
{
	return mpd_info->volume;
}

uint8_t MPClient::get_info_state()
{
	return mpd_info->state;
}

uint16_t MPClient::get_info_elapsed_time()
{
	return mpd_info->elapsed_time;
}

uint8_t MPClient::get_info_kbit_rate()
{
	return mpd_info_song->kbit_rate;
}

uint32_t MPClient::get_info_sample_rate()
{
	return mpd_info_song->sample_rate;
}

uint8_t MPClient::get_info_channels()
{
	return mpd_info_song->channels;
}

uint8_t MPClient::get_info_bits()
{
	return mpd_info_song->bits;
}

std::string MPClient::get_info_title()
{
	return mpd_info_song->title;
}

std::string MPClient::get_info_album()
{
	return mpd_info_song->album;
}

std::string MPClient::get_info_artist()
{
	return mpd_info_song->artist;
}

