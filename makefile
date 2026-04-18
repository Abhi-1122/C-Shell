# Variables
CC      := gcc
CFLAGS  := -std=c99 \
           -D_POSIX_C_SOURCE=200809L \
           -D_XOPEN_SOURCE=700 \
           -Wall -Wextra -Werror \
           -Wno-unused-parameter \
           -fno-asm \
           -Iinclude

SRC_DIR := src
SRC     := $(wildcard $(SRC_DIR)/*.c)
OUT     := shell.out

all: $(OUT)
	./$(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

