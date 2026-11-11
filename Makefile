CC ?= cc
CFLAGS ?= -std=c17 -Wall -Wextra -Werror -pedantic -O2
SOURCES = src/main.c src/schedule.c src/print.c src/server.c src/client.c \
          src/policy.c src/nfs.c src/afs.c

.PHONY: all clean
all: build/ps4

build/ps4: $(SOURCES) include/ps4.h | build
	$(CC) $(CFLAGS) -Iinclude $(SOURCES) -o $@

build:
	mkdir -p build

clean:
	rm -f build/ps4
	rmdir build 2>/dev/null || true
