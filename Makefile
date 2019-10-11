#executable name
TARGET = tortuga

#compiler options
COMPILER = g++
FLAGS = -DDEBUG_MODE -g -std=c++17 -pthread -Wall -Wno-narrowing
PATHS = -Iusr/includes/ -Lusr/libs/
LIBS = -lvulkan -lglfw -lwayland-client -lm

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
	$(COMPILER) -o $(OBJ_DIR)/$(TARGET) $(SRC_EXECUTABLE) $(FLAGS) $(PATHS) $(LIBS) $(OBJ_FILES) $(LIBS_A)

#create obj files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p "$(@D)"
	$(COMPILER) -c -o $@ $< $(FLAGS) $(PATHS) $(LIBS) $(LIBS_A)

clean:
	rm -rf Build

submodule:
	mkdir -p usr
	#init
	git submodule init
	git submodule update --recursive --init
	#vulkan headers
	mkdir -p submodules/Vulkan-Headers/build
	cd submodules/Vulkan-Headers/build && cmake -DCMAKE_INSTALL_PREFIX=$(PWD)/usr ..
	make install -C submodules/Vulkan-Headers/build
	#vulkan loader
	mkdir -p submodules/Vulkan-Loader/build
	echo 'set(VULKAN_HEADERS_INSTALL_DIR "$(PWD)/usr" CACHE STRING "" FORCE)' > $(PWD)/submodules/Vulkan-Loader/build/helper.cmake
	cd submodules/Vulkan-Loader/build && cmake -C helper.cmake -DCMAKE_INSTALL_PREFIX=$(PWD)/usr ..
	make -C submodules/Vulkan-Loader/build
	make install -C submodules/Vulkan-Loader/build
	#glslang
	

	#glfw
	#mkdir -p submodules/glfw/build
	#cd submodules/glfw/build && cmake ..
	#make -C submodules/glfw/build