CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -lraylib -fsanitize=address \
	-Wno-error=unused-variable -Wno-error=unused-parameter -Wno-error=parentheses

BUILD = build
OUTPUT = $(BUILD)/pacman
OUTPUT_ARGS =

CSRCS = $(shell find src -name '*.c')
OBJS = $(patsubst %.c,$(BUILD)/%.o,$(CSRCS))

all: clean $(OBJS) $(OUTPUT) assets run

force:

$(BUILD)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $(OUTPUT)

assets: force
	cp -R assets $(BUILD)/assets

run:
	./$(OUTPUT) $(OUTPUT_ARGS)

clean:
	rm -rf $(BUILD)