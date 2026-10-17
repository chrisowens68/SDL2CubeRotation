SRC_DIR = src
BUILD_DIR = build/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g
LINKER_FLAGS = -lsdl2

.PHONY: all run clean

all:
	mkdir -p $(BUILD_DIR)
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(SRC_FILES) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(BUILD_DIR)/$(OBJ_NAME)

run: all
	./$(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -rf $(BUILD_DIR)/$(OBJ_NAME) $(BUILD_DIR)/$(OBJ_NAME).dSYM
