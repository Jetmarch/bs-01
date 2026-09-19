CC = clang

TARGET_TRIPLE = x86_64-w64-windows-gnu
TARGET = build/game
SOURCE = src/gol.c

CFLAGS = -target $(TARGET_TRIPLE) \
		 -std=c17 -Wall -Wextra -Wpedantic -O0 -g \
         -Ivendor/raylib/src

LDFLAGS = -Lvendor/raylib/src


# ============================================================
# Windows / MinGW
# ============================================================

ifeq ($(OS),Windows_NT)

    PLATFORM = WINDOWS

    CFLAGS += -DPLATFORM_DESKTOP

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
