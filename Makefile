#executable name
TARGET = tortuga

#compiler options
COMPILER = g++
FLAGS = -g -std=c++17 -pthread -Wall -DDEBUG_MODE -Wno-narrowing
INCLUDE = -Isubmodules/includes/
LIBS = -Lsubmodules/libs/ -lvulkan -lglfw -lwayland-client

#important paths
SRC_DIR = Source
OBJ_DIR = Build
SRC_EXECUTABLE = Source/main.cpp

#get a list of all cpp files excluding executable file
SRC_FILES := $(shell find $(SRC_DIR)/ -type f -name '*.cpp' ! -path '$(SRC_EXECUTABLE)')
#get a list of all obj files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

#link and create executable
all: $(OBJ_FILES)
	cp -r Assets $(OBJ_DIR)/Assets
	$(COMPILER) -o $(OBJ_DIR)/$(TARGET) $(SRC_EXECUTABLE) $(FLAGS) $(INCLUDE) $(LIBS) $(OBJ_FILES)

#create obj files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p "$(@D)"
	$(COMPILER) -c -o $@ $< $(FLAGS) $(INCLUDE) $(LIBS)

clean:
	rm -rf Build