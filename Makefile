CC = gcc

SOURCE = $(wildcard src/*.c)
OBJ = $(SOURCE:src/%.c=obj/%.o)

CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O0 -g -MMD -MP \
         -Ivendor/raylib/src -isystem vendor

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

    TARGET := build/game

endif


# ============================================================
# Targets
# ============================================================

.PHONY: all run clean info setup

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p build
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)
	cp -r resources build/

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build obj

info:
	@echo "Platform: $(PLATFORM)"
	@echo "Compiler: $(CC)"
	@echo "Target:   $(TARGET)"

setup:
	git submodule update --init --recursive
