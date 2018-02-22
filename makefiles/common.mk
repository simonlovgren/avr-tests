# Globally common variables
PROGRAMMER := buspirate

# Detect OS and set correct variables
ifeq ($(OS),Windows_NT)
	# Windows
	PORT := COM3
	RM   := del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# Linux
		PORT := /dev/ttyUSB0
		RM   := rm -f
	endif
	ifeq ($(UNAME_S),Darwin)
		# OSX
		PORT := /dev/ttyUSB0
		RM   := rm -f
	endif
endif