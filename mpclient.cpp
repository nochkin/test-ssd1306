#include <mpd/connection.h>
#include <mpd/status.h>
#include <mpd/list.h>
#include <mpd/player.h>
#include <mpd/response.h>
#include <mpd/song.h>

#include <mpclient.h>

MPClient::MPClient(char *host, uint16_t port)
{
	mpd_host = host;
	mpd_port = port;
}

bool MPClient::connect()
{
	mpd_conn = mpd_connection_new(0, 0, 30000);
	if (mpd_connection_get_error(mpd_conn) != MPD_ERROR_SUCCESS) {
		return false;
	}
	return true;
}

void MPClient::update_status()
{
	mpd_command_list_begin(mpd_conn, true);
	mpd_send_status(mpd_conn);
	mpd_send_current_song(mpd_conn);
	mpd_command_list_end(mpd_conn);
	mpd_status = mpd_recv_status(mpd_conn);
	if (mpd_status == 0) { // error
	} else {
		mpd_info->volume = mpd_status_get_volume(mpd_status);
		mpd_info->state = mpd_status_get_state(mpd_status);
		if (mpd_info->state == MPD_STATE_PLAY || mpd_info->state == MPD_STATE_PAUSE) {
			mpd_info->elapsed_time = mpd_status_get_elapsed_time(mpd_status);
			mpd_info->kbit_rate = mpd_status_get_kbit_rate(mpd_status);
		}
		mpd_audio_format = mpd_status_get_audio_format(mpd_status);

		mpd_status_free(mpd_status);

		if (mpd_connection_get_error(mpd_conn) != MPD_ERROR_SUCCESS) {
			// errror
		} else {
			mpd_response_next(mpd_conn);
			mpd_song = mpd_recv_song(mpd_conn);
		}
	}
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
	return mpd_info->elapsed_time;
}

std::string MPClient::get_info_title()
{
	return mpd_info->title;
}

