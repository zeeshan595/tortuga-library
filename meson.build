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


tortuga_inc = include_directories('Source')
tortuga_lib = library(
	'tortuga',
	'Source/Tortuga.cpp',
	dependencies: [thread_dep, vulkan_dep, sdl2_dep],
	install: true
)

tortuga_dep = declare_dependency(
	include_directories: tortuga_inc,
	link_with: tortuga_lib
)
