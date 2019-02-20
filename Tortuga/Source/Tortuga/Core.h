#define DEBUG_MODE true
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

//General
#include <array>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <optional>
#include <set>
#include <pthread.h>
#include <exception>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//3rd Party
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL_image.h>
