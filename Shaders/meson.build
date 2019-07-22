
shaders_src = [
	'ray-marching.comp'
]

shaders = []
glslang = find_program('glslangValidator')

foreach shader : shaders_src
	spv = custom_target(
		shader + '_spv',
		output: shader + '.spv',
		input: shader,
		command: [glslang, '-V', '@INPUT@', '-o', '@OUTPUT@'],
		install_dir: '../Build/Shaders',
		install: true
	)

	shaders += [spv]
endforeach