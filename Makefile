SRC_DIR := src
TEST_DIR := test_assembly
OUT_DIR := out

SOURCES := $(wildcard $(SRC_DIR)/*.c)
BIN     := 6502emu

.PHONY: always clean build install assemble_test
always:
	mkdir -p $(OUT_DIR)
clean:
	rm -r $(OUT_DIR)
build: always assemble_test $(OUT_DIR)/$(BIN)
assemble_test: always $(OUT_DIR)/test

$(OUT_DIR)/$(BIN): $(SOURCES)
	gcc -o $@ $^ -lncurses

$(OUT_DIR)/test: $(TEST_DIR)/test.s
	ca65 $^ -o $@.o -l $@.lst
	ld65 -v -vm -m $@.map -C $(TEST_DIR)/test.cfg -o $@ $@.o

install: build
	install -d /usr/local/bin
	install -C -b $(OUT_DIR)/$(BIN) /usr/local/bin
