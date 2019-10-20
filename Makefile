#executable name
TARGET = tortuga

#os specific options
ifeq ($(OS),Windows_NT)
	#windows display servers
	DISPLAY_SERVER = -DVK_USE_PLATFORM_WIN32_KHR
	DISPLAY_SERVER_LIB = 
else
	UNAME_S := $(shell uname -s)
	#linux display servers
	ifeq ($(UNAME_S),Linux)
		ifeq ($(XDG_SESSION_TYPE),wayland)
			DISPLAY_SERVER = -DVK_USE_PLATFORM_WAYLAND_KHR
			DISPLAY_SERVER_LIB = -lwayland-client
		else
			DISPLAY_SERVER = -DVK_USE_PLATFORM_XCB_KHR
			DISPLAY_SERVER_LIB = 
		endif
	endif
	#darwin display servers
	ifeq ($(UNAME_S),Darwin)
		#todo: mac display servers
	endif
endif
#compiler options
COMPILER = g++
PRE_PROCESSOR = $(DISPLAY_SERVER) -DGLM_FORCE_RADIANS -DGLM_FORCE_DEPTH_ZERO_TO_ONE -DSTB_IMAGE_IMPLEMENTATION -DSTB_IMAGE_STATIC
FLAGS = -DDEBUG_MODE -g -std=c++17 -pthread -Wall -Wno-narrowing -Wno-unused $(PRE_PROCESSOR)
PATHS = -IBuild/include/ -LBuild/lib64/ -LBuild/lib/
LIBS = -lvulkan $(DISPLAY_SERVER_LIB)

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
	rm -rf Build/Tortuga
	rm -rf Build/tortuga

init:
	mkdir -p Build
	#init
	git submodule init
	git submodule update --recursive --init
	#wayland
	cd Submodules/wayland && sh autogen.sh --disable-documentation --prefix=$(PWD)/Build
	make -C Submodules/wayland
	make install -C Submodules/wayland
	#wayland protocols
	cd Submodules/wayland-protocols && sh autogen.sh --disable-documentation --prefix=$(PWD)/Build
	make -C Submodules/wayland-protocols
	make install -C Submodules/wayland-protocols
	Build/bin/wayland-scanner client-header Build/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml Build/include/xdg-decoration.h
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