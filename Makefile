TARGET = partial_life

BUILD_DIR = build

CC = clang

EMCC = emcc
EMCCFLAGS = -Wall -Wextra -std=c23 -pedantic -O3 \
            -Iraylib/src -Irini/src \
            -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
EMLDFLAGS = -s USE_GLFW=3 \
						-s WASM=1 \
						-s ASYNCIFY \
						-s GL_ENABLE_GET_PROC_ADDRESS=1 \
						-s TOTAL_MEMORY=67108864 \
						-s STACK_SIZE=8388608 \
						-s ALLOW_MEMORY_GROWTH=1 \
						--shell-file raylib/src/shell.html \

						#--preload-file resources \

#  -Weverything
CCFLAGS = -Wall -Wextra -std=c23 -pedantic -O2 -g3 -ggdb -gdwarf-4 -Iraylib/src -Irini/src
LDFLAGS = 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	RAYLIB_LIBS = -framework OpenGL -framework Cocoa -framework IOKit \
	              -framework CoreVideo -framework QuartzCore -framework Foundation
else
	$(error Not implemented)
endif

RAYLIB_STATIC = raylib/src/libraylib.a
RAYLIB_WEB    = raylib/src/libraylib.web.a

all: _build

$(RAYLIB_STATIC):
	$(MAKE) -C raylib/src PLATFORM=PLATFORM_DESKTOP

$(RAYLIB_WEB):
	$(MAKE) -C raylib/src PLATFORM=PLATFORM_WEB

_build: $(RAYLIB_STATIC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CCFLAGS) src/main.c -o $(BUILD_DIR)/$(TARGET) $(RAYLIB_STATIC) $(RAYLIB_LIBS) $(LDFLAGS)

web: $(RAYLIB_WEB)
	@mkdir -p $(BUILD_DIR)
	$(EMCC) $(EMCCFLAGS) src/main.c -o $(BUILD_DIR)/$(TARGET).html $(RAYLIB_WEB) $(EMLDFLAGS)

compile_commands.json: clean
	bear -- make

clean:
	$(MAKE) -C raylib/src clean
	rm -f $(BUILD_DIR)

run: _build
	./$(BUILD_DIR)/$(TARGET)
