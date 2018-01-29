CC = gcc
CFLAGS = -g -std=gnu99 -fPIC
LDFLAGS = -shared
INCLUDES = -I..

SRC_DIR = ./
OBJ_DIR = ./obj

TARGET = $(OBJ_DIR)/libcgds.so

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
H_FILES = $(wildcard $(SRC_DIR)/*.h)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
