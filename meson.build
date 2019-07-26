project(
	'tortuga-library',
	'cpp',
	version: '0.1.0',
	meson_version: '>= 0.42.0'
)

add_project_arguments(
	'-Wall',
	'-std=c++17',
	'-DDEBUG_MODE',
	'-Wno-narrowing',
	language: 'cpp')

thread_dep = dependency('threads')
vulkan_dep = dependency('vulkan')
sdl2_dep = dependency('SDL2')
shaderc = dependency('shaderc')

subdir('Source')
tortuga_inc = include_directories('Source')
tortuga_lib = library(
	'tortuga',
	sourceCppFiles,
	dependencies: [thread_dep, vulkan_dep, sdl2_dep, shaderc]
)

tortuga_dep = declare_dependency(
	include_directories: tortuga_inc,
	link_with: tortuga_lib
)

exe = executable(
	'tortuga',
	'Source/main.cpp',
	dependencies: [tortuga_dep, thread_dep, vulkan_dep, sdl2_dep, shaderc]
)