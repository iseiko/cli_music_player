CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

SRC = \
  src/main.c \
  src/audio/audio_dummy.c \
  src/cli/cli.c \
  src/core/state.c \
  src/player/player.c \
  src/playlist/playlist.c

INC = -Isrc

OUT = music_cli

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)

.PHONY: all clean