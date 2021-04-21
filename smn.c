#include "client.h"
#include <stdlib.h>

int main()
{
	struct mpd_connection *conn = get_connection();
	struct mpd_status* status = NULL;
	struct mpd_song* song = NULL;
	int song_id = -1;
	// Mainloop.
	bool running = true;
	while (running)
	{
		running = update(conn, status, &song_id);
	}
	return 0;
}
