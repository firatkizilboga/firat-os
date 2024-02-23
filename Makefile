.PHONY: build emulate

build:
	python build.py

emulate:
	qemu-system-i386 -kernel build/kernel.bin

all: build emulate
