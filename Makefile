TARGET = partial_life


CC = clang

CCFLAGS = -Wall -Wextra -std=c23 -pedantic -Weverything -Iraylib/src
LDFLAGS = 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	RAYLIB_LIBS = -framework OpenGL -framework Cocoa -framework IOKit \
	              -framework CoreVideo -framework QuartzCore -framework Foundation
else
	$(error Not implemented)
endif

RAYLIB_STATIC = raylib/src/libraylib.a

all: build

$(RAYLIB_STATIC):
	$(MAKE) -C raylib/src PLATFORM=PLATFORM_DESKTOP

build: $(RAYLIB_STATIC)
	$(CC) $(CCFLAGS) src/main.c -o $(TARGET) $(RAYLIB_STATIC) $(RAYLIB_LIBS) $(LDFLAGS)

compile_commands.json: clean
	bear -- make

clean:
	$(MAKE) -C raylib/src clean
	rm -f $(TARGET)

run: build
	./$(TARGET)
