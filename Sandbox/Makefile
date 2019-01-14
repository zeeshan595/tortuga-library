#CONFIG
PROJECT_NAME = Sandbox
COMPILER = g++
FLAGS = -Wall -g -std=c++17 -pthread -lvulkan -lSDL2
SOURCE_DIR = Source
BUILD_DIR = Build
MAIN_PATH = $(SOURCE_DIR)/Application.cpp

#ENGINE
ENGINE_INCLUDE_PATH = -I../Tortuga/Source/
ENGINE_LIB_PATH = ../Tortuga/Build/libTortuga.a

#FILES
SOURCE_FILES = $(filter-out $(MAIN_PATH), $(shell find $(SOURCE_DIR) -type f -name '*.cpp'))
OBJ_FILES = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_FILES))

INCLUDES = -I../Includes
Libraries = -L../Libs

all: setup exe
setup:
	mkdir -p $(BUILD_DIR)
clean: 
	rm -rf $(BUILD_DIR)

#CREATE EXECUTABLE
exe: $(OBJ_FILES)
	$(COMPILER) $(MAIN_PATH) $(FLAGS) -o $(BUILD_DIR)/Application $(OBJ_FILES) $(ENGINE_INCLUDE_PATH) $(ENGINE_LIB_PATH) $(INCLUDES) $(Libraries)

#CREATE OBJ FILES
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p "$(@D)"
	$(COMPILER) -c $< -o $@