CC			= gcc
CFLAGS 		= -w
SRC_DIR		= src
SOURCES		= $(SRC_DIR)/observable.c $(SRC_DIR)/main.c
OUT_DIR		= bin
EXECUTABLE	= observable

all: 
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUT_DIR)/$(EXECUTABLE)

clean:
	rm -rf $(OUT_DIR)/$(EXECUTABLE)

.DEFAULT_GOAL := all
