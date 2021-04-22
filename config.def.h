// Connection settings.
static const char *host = "localhost";
static const int port = 6600;
static const int timeout_ms = 0;

// The commands to execute when the song changes.
static const char *commands[] = {
	"/home/iota/.local/bin/on_song_change"
};
// The delay between updates in microseconds.
static const int update_delay = 500000;
