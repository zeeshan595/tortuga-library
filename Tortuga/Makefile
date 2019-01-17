#CONFIG
PROJECT_NAME = Tortuga
COMPILER = g++
FLAGS = -Wall -g -std=c++17
SOURCE_DIR = Source
BUILD_DIR = Build

#FILES
SOURCE_FILES = $(shell find $(SOURCE_DIR)/ -type f -name '*.cpp')
OBJ_FILES = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_FILES))

all: setup lib
setup: 
	mkdir -p $(BUILD_DIR)
clean: 
	rm -rf $(BUILD_DIR)

#CREATE STATIC LIBRARY
lib: $(OBJ_FILES)
	ar rcs $(BUILD_DIR)/lib$(PROJECT_NAME).a $(OBJ_FILES)

#CREATE OBJ FILES
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	echo $<
	mkdir -p "$(@D)"
	$(COMPILER) -c $< -o $@ $(FLAGS)

$(SOURCE_DIR)/%.cpp:
	echo $<