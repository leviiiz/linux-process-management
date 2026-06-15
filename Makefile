CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lpthread

.PHONY: all clean

all:
	@echo "Build ready"

clean:
	rm -rf bin obj