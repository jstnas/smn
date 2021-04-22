#include "smn.h"

bool get_connection(struct mpd_connection **conn) {
	*conn = mpd_connection_new(host, port, timeout_ms);
	if (conn == NULL) {
		printf("Out of memory for connection\n");
		return false;
	}
	else if (mpd_connection_get_error(*conn) != MPD_ERROR_SUCCESS) {
		printf("%s\n", mpd_connection_get_error_message(*conn));
		mpd_connection_free(*conn);
		return false;
	}
	return true;
}

bool get_status(struct mpd_status **status, struct mpd_connection *conn) {
	*status = mpd_run_status(conn);
	if (status == NULL) {
		printf("Failed to get status\n");
		return false;
	}
	return true;
}

void on_song_change() {
	for (int c = 0; c < command_len; c++)
		system(commands[c]);
}

bool update(struct mpd_connection *conn, struct mpd_status *status, int *song_id) {
	if (!get_status(&status, conn))
		return false;
	const int new_song_id = mpd_status_get_song_id(status);
	// Check if the song has changed
	if (*song_id != new_song_id && new_song_id != -1) {
		*song_id = new_song_id;
		on_song_change();
	}
	mpd_status_free(status);
	usleep(update_delay);
	return true;
}

void close_connection(struct mpd_connection *conn) {
	mpd_connection_free(conn);
}

int main() {
	// Init.
	struct mpd_connection *conn = NULL;
	struct mpd_status* status = NULL;
	int song_id = -1;
	if (!get_connection(&conn))
		return 1;
	// Mainloop.
	bool running = true;
	while (running)
		running = update(conn, status, &song_id);
	// Cleanup.
	close_connection(conn);
	return 0;
}
