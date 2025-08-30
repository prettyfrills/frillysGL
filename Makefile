# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g -fdiagnostics-color=always -I./include

# Linker flags
LDFLAGS = -L./lib
LDLIBS = -lassimp -lglfw3dll -lopengl32 -lgdi32

# Source files
CPP_SRCS := $(wildcard src/*.cpp)
C_SRCS := src/glad.c
SRCS := $(CPP_SRCS) $(C_SRCS)

# Output
OUT = main.exe

# Default target
all:
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) $(LDLIBS) -o $(OUT)

# Clean up
clean:
	del /Q main.exe 2>nul || rm -f main.exe

.PHONY: all clean