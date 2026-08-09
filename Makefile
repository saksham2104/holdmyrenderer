.PHONY: all build run clean

all: build run

build:
	cmake -S . -B build
	cmake --build build

run:
	cd . && ./build/tinyrenderer && open main.tga

clean:
	rm -rf build