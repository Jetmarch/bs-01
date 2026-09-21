CC = clang


TARGET = build/game
SOURCE = src/main.c

CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O0 -g -gcodeview \
         -Ivendor/raylib/src -isystem vendor

LDFLAGS = -Lvendor/raylib/src


# ============================================================
# Windows / MinGW
# ============================================================

ifeq ($(OS),Windows_NT)

    PLATFORM = WINDOWS
    TARGET_TRIPLE = x86_64-w64-windows-gnu
    CFLAGS += -DPLATFORM_DESKTOP -target $(TARGET_TRIPLE)

    LDFLAGS += -lraylib \
               -lopengl32 \
               -lgdi32 \
               -lwinmm

    TARGET := build/game.exe

else

# ============================================================
# Linux
# ============================================================

    PLATFORM = LINUX

    LDFLAGS += -lraylib \
               -lm \
               -lpthread \
               -ldl \
               -lrt \
               -lX11

endif


# ============================================================
# Targets
# ============================================================

.PHONY: all run clean info

all: $(TARGET)


$(TARGET): $(SOURCE)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LDFLAGS)
	cp -r resources build/


run: $(TARGET)
	./$(TARGET)


clean:
	rm -rf build


info:
	@echo "Platform: $(PLATFORM)"
	@echo "Compiler: $(CC)"
	@echo "Target:   $(TARGET)"

setup:
	git submodule update --init --recursive
