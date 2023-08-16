SRC_DIR := src
TEST_DIR := test_assembly
OUT_DIR := out

LIBSOURCES 	:= $(SRC_DIR)/cpu.c
LIBOBJECTS 	:= $(patsubst $(SRC_DIR)/%.c, $(OUT_DIR)/%.o, $(LIBSOURCES))
BIN					:= 6502emu
LIB     		:= lib6502emu.a

.PHONY: always clean build install assemble_test
always:
	@echo $(LIBOBJECTS)
	mkdir -p $(OUT_DIR)
clean:
	rm -r $(OUT_DIR)
build: always assemble_test $(OUT_DIR)/$(LIB) $(OUT_DIR)/$(BIN)
assemble_test: always $(OUT_DIR)/test

$(OUT_DIR)/$(BIN): src/tui_main.c
	gcc -o $@ $^ -L$(OUT_DIR) -lncurses -l6502emu

$(OUT_DIR)/$(LIB): $(LIBOBJECTS)
	ar rcs $@ $^

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	gcc -c $^ -o $@

$(OUT_DIR)/test: $(TEST_DIR)/test.s
	ca65 $^ -o $@.o -l $@.lst
	ld65 -v -vm -m $@.map -C $(TEST_DIR)/test.cfg -o $@ $@.o

install: build
	install -d /usr/local/bin
	install -C -b $(OUT_DIR)/$(BIN) /usr/local/bin
	install -d /usr/local/include/lib6502
	install -C -m 644 $(SRC_DIR)/*.h /usr/local/include/lib6502
	install -d /usr/local/lib
	install -C -b $(OUT_DIR)/$(LIB) /usr/local/lib
