#executable name
TARGET = tortuga

#compiler options
COMPILER = g++
FLAGS = -DDEBUG_MODE -g -std=c++17 -pthread -Wall -Wno-narrowing -Wno-unused
PATHS = -IBuild/include/ -LBuild/lib64/ -LBuild/lib/
LIBS = -lvulkan -lglfw

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
	$(COMPILER) -o $(OBJ_DIR)/$(TARGET) $(SRC_EXECUTABLE) $(FLAGS) $(PATHS) $(LIBS) $(OBJ_FILES)

#create obj files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p "$(@D)"
	$(COMPILER) -c -o $@ $< $(FLAGS) $(PATHS) $(LIBS)

clean:
	rm -rf Build

init:
	mkdir -p Build
	#init
	git submodule init
	git submodule update --recursive --init
	#wayland
	cd Submodules/wayland && sh autogen.sh --disable-documentation --prefix=$(PWD)/Build
	make -C Submodules/wayland
	make install -C Submodules/wayland
	#vulkan headers
	mkdir -p Submodules/Vulkan-Headers/build
	cd Submodules/Vulkan-Headers/build && cmake -DCMAKE_INSTALL_PREFIX=$(PWD)/Build ..
	make install -C Submodules/Vulkan-Headers/build
	rm -rf Submodules/glslang/build
	#vulkan loader
	mkdir -p Submodules/Vulkan-Loader/build
	rm -rf $(PWD)/Submodules/Vulkan-Loader/build/helper.cmake
	echo 'set(VULKAN_HEADERS_INSTALL_DIR "$(PWD)/Build" CACHE STRING "" FORCE)' > $(PWD)/Submodules/Vulkan-Loader/build/helper.cmake
	echo 'set(WAYLAND_CLIENT_INCLUDE_DIR "$(PWD)/Build" CACHE STRING "" FORCE)' >> $(PWD)/Submodules/Vulkan-Loader/build/helper.cmake
	cd Submodules/Vulkan-Loader/build && cmake -C helper.cmake -DCMAKE_INSTALL_PREFIX=$(PWD)/Build ..
	make install -C Submodules/Vulkan-Loader/build
	rm -rf Submodules/Vulkan-Loader/build
	#glfw
	mkdir -p Submodules/glfw/build
	cd Submodules/glfw/build && cmake -DBUILD_SHARED_LIBS=ON ..
	make -C Submodules/glfw/build
	ln -f -s ../../Submodules/glfw/include/GLFW Build/include/GLFW
	cp Submodules/glfw/build/src/libglfw.so.3.4 Build/lib/libglfw.so
	rm -rf Submodules/glfw/build
	#stb
	ln -f -s ../../Submodules/stb/ Build/include/stb
	#glm
	ln -f -s ../../Submodules/glm/glm Build/include/glm
	#glslang
	mkdir -p Submodules/glslang/build
	cd Submodules/glslang/build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PWD)/Build ..
	make -C Submodules/glslang/build
	make install -C Submodules/glslang/build
	rm -rf Submodules/glslang/build