#ifndef SMN_H
#define SMN_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpd/client.h>
#include "config.def.h"

static const int command_len = sizeof(commands) / sizeof(char *);

bool get_connection(struct mpd_connection **conn);
bool get_status(struct mpd_status **status, struct mpd_connection *conn);
void on_song_change();
bool update(struct mpd_connection *conn, struct mpd_status *status,
			int *song_id);
void close_connection(struct mpd_connection *conn);

#endif
