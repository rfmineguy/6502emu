SRC_DIR := src
OUT_DIR := out

SOURCES := $(wildcard $(SRC_DIR)/*.c)
BIN     := 6502emu

.PHONY: always clean build
always:
	mkdir -p $(OUT_DIR)
clean:
	rm -r $(OUT_DIR)
build: always $(OUT_DIR)/$(BIN)

$(OUT_DIR)/$(BIN): $(SOURCES)
	gcc -o $@ $^ -lncurses
