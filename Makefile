CC=gcc
SRC_DIR=src
SOURCES=$(SRC_DIR)/observable.c $(SRC_DIR)/main.c
OUT_DIR=bin
EXECUTABLE=observable

all: 
	$(CC) $(SOURCES) -o $(OUT_DIR)/$(EXECUTABLE)

clean:
	rm -rf $(OUT_DIR)/*.o

.DEFAULT_GOAL := all
