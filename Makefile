BIN_DIR = /usr/local/bin
LIBS = -lmpdclient

smn: clean
	gcc smn.c -o smn $(LIBS)

clean:
	rm -f smn

install: smn
	mkdir -p $(BIN_DIR)
	cp -f smn $(BIN_DIR)
	chmod 755 $(BIN_DIR)/smn

uninstall:
	rm -r $(DESTDIR)$(PREFIX)/bin/smn
