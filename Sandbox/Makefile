#CONFIG
PROJECT_NAME = Sandbox
COMPILER = g++
FLAGS = -Wall -g -std=c++17
SOURCE_DIR = Source
BUILD_DIR = Build
MAIN_PATH = $(SOURCE_DIR)/Application.cpp

INCLUDES = -I/usr/include
Libraries = -L/usr/lib
LIB_FLAGS = -pthread -lvulkan -lSDL2 -ldl -lsndio -lSDL2_image

#ENGINE
ENGINE_INCLUDE_PATH = -I../Tortuga/Source/
ENGINE_LIB_PATH = ../Tortuga/Build/libTortuga.a

#FILES
SOURCE_FILES = $(filter-out $(MAIN_PATH), $(shell find $(SOURCE_DIR) -type f -name '*.cpp'))
OBJ_FILES = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_FILES))

VERT_SHADERS = $(shell find $(SOURCE_DIR) -type f -name '*.vert')
COMPILED_VERT_SHADERS = $(patsubst $(SOURCE_DIR)/%.vert, $(BUILD_DIR)/%.vert.spv, $(VERT_SHADERS))
FRAG_SHADERS = $(shell find $(SOURCE_DIR) -type f -name '*.frag')
COMPILED_FRAG_SHADERS = $(patsubst $(SOURCE_DIR)/%.frag, $(BUILD_DIR)/%.frag.spv, $(FRAG_SHADERS))

all: setup exe
build: clean setup exe
setup:
	mkdir -p $(BUILD_DIR)
clean: 
	rm -rf $(BUILD_DIR)

#CREATE EXECUTABLE
exe: $(OBJ_FILES) $(COMPILED_VERT_SHADERS) $(COMPILED_FRAG_SHADERS)
	$(COMPILER) $(MAIN_PATH) $(FLAGS) -o $(BUILD_DIR)/Application $(OBJ_FILES) $(ENGINE_INCLUDE_PATH) $(ENGINE_LIB_PATH) $(INCLUDES) $(Libraries) $(LIB_FLAGS)

#CREATE OBJ FILES
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p "$(@D)"
	$(COMPILER) -c $< -o $@

#COMPILE SHADERS
$(BUILD_DIR)/%.vert.spv: $(SOURCE_DIR)/%.vert
	mkdir -p "$(@D)"
	glslangValidator -V $< -o $@

$(BUILD_DIR)/%.frag.spv: $(SOURCE_DIR)/%.frag
	mkdir -p "$(@D)"
	glslangValidator -V $< -o $@