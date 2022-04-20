CACHE_DIR := .cache
OUT_DIR := bin

TARGET := $(OUT_DIR)/turing-machine
SOURCES := src/main.c \
					 src/parse/buffer.c \
					 src/parse/parse.c \
					 src/parse/reader.c \
					 src/turing-machine/format.c \
					 src/turing-machine/machine.c \
					 src/turing-machine/tape.c \
					 src/turing-machine/transition.c \
					 src/util/hash_table.c \
					 src/util/list.c \
					 src/util/map.c
OBJECTS := $(SOURCES:src/%.c=$(CACHE_DIR)/%.o)
DEPS := $(SOURCES:src/%.c=$(CACHE_DIR)/%.d)

CC := gcc
CFLAGS := -std=c11 -Wall -Isrc -D_POSIX_C_SOURCE=1

ifeq ($(UNICODE), 1)
	CFLAGS += -DUNICODE
endif

ifeq ($(ANSI), 1)
	CFLAGS += -DANSI
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG -ggdb
endif

ifeq ($(PROFILE), 1)
	CFLAGS += -pg
endif

# enable overflow checks
ifeq ($(DEBUG), 1)
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif

all: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(CACHE_DIR) $(OUT_DIR)

$(TARGET): $(DEPS) $(OBJECTS) $(OUT_DIR)
	$(LINK.c) -o $@ $(OBJECTS)

$(CACHE_DIR)/%.d: src/%.c
	@mkdir -p $(shell dirname $@)
	$(COMPILE.c) -M -o $@ $<

$(CACHE_DIR)/%.o: src/%.c
	@mkdir -p $(shell dirname $@)
	$(COMPILE.c) -o $@ $<

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

-include $(DEPS)

# Documentation

.PHONY: docs
docs:
	doxygen

# Test

test: tests/tests.sh
	tests/tests.sh

$(CACHE_DIR)/test.out: tests/tests.sh
	tests/tests.sh > $@
