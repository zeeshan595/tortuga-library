PROJECT_NAME = Tortuga
SRC_DIR = Source
BUILD_DIR = Build
COMPILER = g++
FLAGS = -Wall -g -std=c++17

#INCLUDES
INCLUDE_PATH = Includes

#LIBS
VULKAN_PATH = Libs/libvulkan.so
SDL_PATH = Libs/libSDL2.so
PTHREAD_PATH = Libs/libpthread.so

#FILES
SRC_FILES = $(filter-out $(SRC_DIR)/Main.cpp, $(shell find $(SRC_DIR) -type f -name '*.cpp'))
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.so, $(SRC_FILES))

all: clean setup main
setup:
	mkdir -p $(BUILD_DIR)
clean:
	rm -rf $(BUILD_DIR)

main: $(OBJ_FILES)
	$(COMPILER) $(SRC_DIR)/Main.cpp -o $(BUILD_DIR)/$(PROJECT_NAME) $(FLAGS) $^ $(VULKAN_PATH) $(SDL_PATH) $(PTHREAD_PATH) -I$(INCLUDE_PATH)

$(BUILD_DIR)/%.so: $(SRC_DIR)/%.cpp
	mkdir -p "$(@D)"
	$(COMPILER) -c -o $@ $<