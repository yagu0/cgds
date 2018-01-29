CC = gcc
CFLAGS = -g -std=gnu99 -Wno-implicit-function-declaration
LDFLAGS = -L../src/obj -lcgds -Wl,-rpath=../src/obj
INCLUDES = -I..

SRC_DIR = ./
OBJ_DIR = ./obj

TARGET = test

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
H_FILES = lut.h helpers.h
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(H_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
