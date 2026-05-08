CC = gcc
PROFILE ?= DEBUG

CCFLAGS_DEBUG = -ggdb -O0 -fno-builtin -DDEBUG
CCFLAGS_DEBUG = -Ofast

SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin
TEST_DIR := test

TGT_INC_DIR := /usr/include
TGT_BIN_DIR := /usr/lib

SRCS := $(wildcard $(SRC_DIR)/*.c)
DEPS := $(wildcard $(INC_DIR)/*.h)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CCFLAGS += $(CCFLAGS_$(PROFILE)) -I$(INC_DIR) -std=c99 -Wall -Wextra
LDFLAGS += -lc

BINS := $(BIN_DIR)/libjsc.a $(BIN_DIR)/libjsc.so

all: prebuild $(BINS)

install: all
	cp $(BINS) $(TGT_BIN_DIR)
	cp $(INC_DIR)/*.h $(TGT_INC_DIR)

prebuild:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

rebuild: clean all

$(BIN_DIR)/libjsc.a: $(OBJS)
	ar rcs $@ $^

$(BIN_DIR)/libjsc.so: $(SRCS) $(DEPS)
	$(CC) -o $@ $(SRCS) $(DEPS) -shared -fPIC $(CCFLAGS) $(LDFLAGS)
ifeq ($(PROFILE), RELEASE)
	strip ./bin/libjsc.so
endif

$(OBJ_DIR)/%.o: $(SRCS)
	$(CC) -c $< -o $@ $(CCFLAGS)

.PHONY: all install clean rebuild test
