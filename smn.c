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
	printf("Song Change!\n");
	FILE *p;
	for (int c = 0; c < command_len; c++) {
		p = popen(commands[c], "r");
		if (p == NULL) {
			printf("Failed to run %s", commands[c]);
		}
	}
}

bool update(struct mpd_connection *conn, int *song_id) {
	printf("Updating\r");
	struct mpd_status *status = NULL;
	if (!get_status(&status, conn)) {
		printf("Failed to get status\n");
		return false;
	}
	int new_song_id = mpd_status_get_song_id(status);
	// Check if the song has changed
	if (*song_id != new_song_id && new_song_id != -1) {
		*song_id = new_song_id;
		printf("New song id is %i\n", *song_id);
		on_song_change();
	}
	mpd_status_free(status);
	sleep(1);
	return true;
}

void close_connection(struct mpd_connection *conn) {
	mpd_connection_free(conn);
}

int main() {
	printf("Initialising smn\n");
	// Init.
	struct mpd_connection *conn = NULL;
	int song_id = -1;
	if (!get_connection(&conn))
		return 1;
	// Mainloop.
	bool running = true;
	printf("Running mainloop\n");
	while (running)
		running = update(conn, &song_id);
	// Cleanup.
	close_connection(conn);
	printf("Closing smn\n");
	return 0;
}
