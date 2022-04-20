CACHE_DIR := .cache
OUT_DIR := bin

TARGET := $(OUT_DIR)/turing-machine
SOURCES := src/args.c \
					 src/main.c \
					 src/parse/buffer.c \
					 src/parse/parse_alan.c \
					 src/parse/parse_json.c \
					 src/parse/reader.c \
					 src/turing-machine/format.c \
					 src/turing-machine/machine.c \
					 src/turing-machine/tape.c \
					 src/turing-machine/transition.c \
					 src/util/hash_table.c \
					 src/util/list.c \
					 src/util/log.c \
					 src/util/map.c
CC := gcc
CFLAGS := -std=c11 -Wall -Isrc
LDFLAGS := -ljson-c

# config options
ifeq ($(UNICODE), 1)
	CFLAGS += -DUNICODE
endif

ifeq ($(ANSI), 1)
	CFLAGS += -DANSI
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb
else
	CFLAGS += -O3
endif

ifeq ($(PROFILE), 1)
	CFLAGS += -pg
endif

# enable overflow checks
ifeq ($(DEBUG), 1)
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif

UNAME := $(shell uname -s)
ifeq ($(UNAME), Darwin)
	CFLAGS += -isystem /opt/local/include
	LDFLAGS += -L /opt/local/lib -largp
else
	CFLAGS += -D_POSIX_C_SOURCE=1
endif

OBJECTS := $(SOURCES:%.c=$(CACHE_DIR)/%.o)
DEPS := $(SOURCES:%.c=$(CACHE_DIR)/%.d)

all: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(CACHE_DIR) $(OUT_DIR)

$(TARGET): $(DEPS) $(OBJECTS) $(OUT_DIR)
	$(LINK.c) -o $@ $(OBJECTS)

$(CACHE_DIR)/%.d: %.c
	@mkdir -p $(shell dirname $@)
	$(COMPILE.c) -M -o $@ $<

$(CACHE_DIR)/%.o: %.c
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
