CC = gcc

SOURCE = $(wildcard src/*.c)
OBJ = $(SOURCE:src/%.c=obj/%.o)

CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O0 -g\
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



# ============================================================
# Windows / Powershell
# ============================================================

ifeq ($(OS),Windows_NT)

.PHONY: all run clean info setup

all: $(TARGET)

$(TARGET): $(OBJ)
	if not exist build mkdir build
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)
	xcopy /e /Y resources build

obj/%.o: src/%.c
	if not exist obj mkdir obj
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	if exist obj rmdir /s /q obj
	if exist build rmdir /s /q build

# ============================================================
# Linux
# ============================================================

else
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

endif

info:
	@echo "Platform: $(PLATFORM)"
	@echo "Compiler: $(CC)"
	@echo "Target:   $(TARGET)"

setup:
	git submodule update --init --recursive
