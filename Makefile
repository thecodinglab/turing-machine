CACHE_DIR := .cache
OUT_DIR := bin

TARGET := $(OUT_DIR)/turing-machine
SOURCES := src/main.c \
					 src/parse/buffer.c \
					 src/parse/parse.c \
					 src/parse/reader.c \
					 src/turing-machine/format.c \
					 src/turing-machine/machine.c \
					 src/turing-machine/tape.c
OBJECTS := $(SOURCES:src/%.c=$(CACHE_DIR)/%.o)
DEPS := $(SOURCES:src/%.c=$(CACHE_DIR)/%.d)

CC := gcc
CFLAGS := -std=c11 -Wall -Isrc

ifeq ($(UNICODE), 1)
	CFLAGS += -DUNICODE
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG -ggdb
endif

all: $(TARGET)

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

# Test

test: tests/tests.sh
	tests/tests.sh

$(CACHE_DIR)/test.out: tests/tests.sh
	tests/tests.sh > $@
