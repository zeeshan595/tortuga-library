project(
	'tortuga-library',
	'cpp',
	version: '0.1.0',
	meson_version: '>= 0.40.0'
)

add_project_arguments(
	'-Wall',
	'-std=c++17',
	'-DDEBUG_MODE',
	language: 'cpp')

thread_dep = dependency('threads')
vulkan_dep = dependency('vulkan')
sdl2_dep = dependency('SDL2')

sourceCppFiles = [
	'Source/Tortuga/Console.cpp',
	'Source/Tortuga/Utils/InputOutput.cpp',

	'Source/Tortuga/Graphics/VulkanBuffer.cpp',
	'Source/Tortuga/Graphics/VulkanCommand.cpp',
	'Source/Tortuga/Graphics/VulkanDevice.cpp',
	'Source/Tortuga/Graphics/VulkanErrorHandler.cpp',
	'Source/Tortuga/Graphics/VulkanFence.cpp',
	'Source/Tortuga/Graphics/VulkanImage.cpp',
	'Source/Tortuga/Graphics/VulkanInstance.cpp',
	'Source/Tortuga/Graphics/VulkanPipeline.cpp',
	'Source/Tortuga/Graphics/VulkanSemaphore.cpp',
	'Source/Tortuga/Graphics/VulkanShader.cpp',
	'Source/Tortuga/Graphics/VulkanSwapchain.cpp',
	'Source/Tortuga/Graphics/Window.cpp'
]

tortuga_inc = include_directories('Source')
tortuga_lib = library(
	'tortuga',
	sourceCppFiles,
	dependencies: [thread_dep, vulkan_dep, sdl2_dep],
	install: true
)

tortuga_dep = declare_dependency(
	include_directories: tortuga_inc,
	link_with: tortuga_lib
)

exe = executable(
	'tortuga',
	'Source/main.cpp',
	dependencies: [tortuga_dep, thread_dep, vulkan_dep, sdl2_dep],
	install: true
)
