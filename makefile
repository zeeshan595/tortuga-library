all:
	make -C Tortuga/
	make -C Editor/
build:
	make build -C Tortuga/
	make build -C Editor/