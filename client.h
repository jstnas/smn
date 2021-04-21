#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpd/client.h>
#include "config.def.h"

struct mpd_connection *get_connection()
{
	struct mpd_connection *conn = mpd_connection_new(NULL, 0, 0);
	if (conn == NULL)
	{
		printf("Out of memory for connection\n");
		return NULL;
	}
	else if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
	{
		printf("%s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return NULL;
	}
	return conn;
}

struct mpd_status *get_status(struct mpd_connection *conn)
{
	struct mpd_status *status = mpd_run_status(conn);
	if (status == NULL)
	{
		printf("Failed to get status\n");
		return NULL;
	}
	return status;
}

void on_song_change()
{
	const int command_len = sizeof(commands) / sizeof(char *);
	for (int c = 0; c < command_len; c++)
	{
		system(commands[c]);
	}
}

bool update(struct mpd_connection *conn, struct mpd_status *status, int *song_id)
{
	status = get_status(conn);
	if (status == NULL)
	{
		return false;
	}
	const int new_song_id = mpd_status_get_song_id(status);
	// Check if the song has changed
	if (*song_id != new_song_id && new_song_id != -1)
	{
		*song_id = new_song_id;
		on_song_change();
	}
	mpd_status_free(status);
	sleep(1);
	return true;
}

#endif
