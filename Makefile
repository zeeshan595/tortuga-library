#executable name
TARGET = tortuga

#compiler options
COMPILER = g++
FLAGS = -DDEBUG_MODE -g -std=c++17 -pthread -Wall -Wno-narrowing
PATHS = -Iusr/include/ -Lusr/lib64/
LIBS = -lvulkan -lglfw

#important paths
SRC_DIR = Source
OBJ_DIR = Build
SRC_EXECUTABLE = Source/main.cpp

#get a list of all cpp files excluding executable file
SRC_FILES := $(shell find $(SRC_DIR)/ -type f -name '*.cpp' ! -path '$(SRC_EXECUTABLE)')
#get a list of all obj files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
#lib a files
LIBS_A := $(shell find usr/lib64/ -type f -name '*.a')

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

init:
	mkdir -p usr
	#init
	git submodule init
	git submodule update --recursive --init
	#vulkan headers
	mkdir -p Submodules/Vulkan-Headers/build
	cd Submodules/Vulkan-Headers/build && cmake -DCMAKE_INSTALL_PREFIX=$(PWD)/usr ..
	make install -C Submodules/Vulkan-Headers/build
	#vulkan loader
	mkdir -p Submodules/Vulkan-Loader/build
	echo 'set(VULKAN_HEADERS_INSTALL_DIR "$(PWD)/usr" CACHE STRING "" FORCE)' > $(PWD)/Submodules/Vulkan-Loader/build/helper.cmake
	cd Submodules/Vulkan-Loader/build && cmake -C helper.cmake -DCMAKE_INSTALL_PREFIX=$(PWD)/usr ..
	make install -C Submodules/Vulkan-Loader/build
	#glfw
	mkdir -p Submodules/glfw/build
	cd Submodules/glfw/build && cmake -DBUILD_SHARED_LIBS=ON ..
	make -C Submodules/glfw/build
	ln -f -s ../../Submodules/glfw/include/GLFW usr/include/GLFW
	ln -f -s ../../Submodules/glfw/build/src/libglfw.so usr/lib64/libglfw.so
	#stb
	ln -f -s ../../Submodules/stb/ usr/include/stb
	#glm
	ln -f -s ../../Submodules/glm/glm usr/include/glm
	#glslang
	mkdir -p Submodules/glslang/build
	cd Submodules/glslang/build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PWD)/usr ..
	make -C Submodules/glslang/build
	make install -C Submodules/glslang/build