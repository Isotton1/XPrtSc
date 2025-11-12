# XPrtSc - X Print Screen

.POSIX:

include config.mk

SRC = xprtsc.c
OBJ = $(SRC:.c=.o)

all: xprtsc

.c.o:
	$(CC) -c $(CFLAGS) $<

$(OBS): config.mk

xprtsc: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f xprtsc $(OBJ) xprtsc-$(VERSION).tar.gz

dist: clean
	mkdir -p xprtsc-$(VERSION)
	cp -R LICENSE Makefile README.md config.mk $(SRC) xprtsc-$(VERSION)
	tar -cf xprtsc-$(VERSION).tar xprtsc-$(VERSION)
	gzip xprtsc-$(VERSION).tar
	rm -r xprtsc-$(VERSION)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp xprtsc $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/xprtsc

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/xprtsc

.PHONY: all clean dist install uninstall
