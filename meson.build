project(
	'tortuga-library',
	'cpp',
	version: '0.1.0',
	meson_version: '>= 0.40.0'
)

thread_dep = dependency('threads')

tortuga_inc = include_directories('Source')
tortuga_lib = library(
	'tortuga',
	'Source/Tortuga.cpp',
	cpp_args: ['-g', '-Wall', '-std=c++17', '-DDEBUG_MODE'],
	link_args : ['-pthread', '-lvulkan', '-lSDL2'],
	dependencies: [thread_dep]
)

tortuga_dep = declare_dependency(include_directories: tortuga_inc, link_with: tortuga_lib)
