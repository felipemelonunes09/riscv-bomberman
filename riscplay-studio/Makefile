ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
PROJECT_DIR := $(ROOT)/examples/bomberman
SDK_DIR := $(ROOT)/sdk
BUILD_DIR ?= $(ROOT)/build/bomberman
SERVER_BUILD_DIR ?= $(ROOT)/dist/builds

CROSS_COMPILE ?= $(shell if [ -x "$(ROOT)/toolchain/bin/riscv-none-elf-gcc" ]; then \
	printf '%s' '$(ROOT)/toolchain/bin/riscv-none-elf-'; \
elif [ -x "$(ROOT)/../toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin/riscv-none-elf-gcc" ]; then \
	printf '%s' '$(ROOT)/../toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin/riscv-none-elf-'; \
elif command -v riscv-none-elf-gcc >/dev/null 2>&1; then \
	printf '%s' 'riscv-none-elf-'; \
elif command -v riscv64-unknown-elf-gcc >/dev/null 2>&1; then \
	printf '%s' 'riscv64-unknown-elf-'; \
elif command -v riscv64-elf-gcc >/dev/null 2>&1; then \
	printf '%s' 'riscv64-elf-'; \
elif command -v riscv32-unknown-elf-gcc >/dev/null 2>&1; then \
	printf '%s' 'riscv32-unknown-elf-'; \
else \
	printf '%s' 'riscv-none-elf-'; \
fi)

CC := $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
ARCH_FLAGS := -march=rv32i -mabi=ilp32 -mno-relax
CFLAGS := -Os -std=gnu99 -ffreestanding -fno-builtin \
	-fno-delete-null-pointer-checks -fno-jump-tables -fno-pic -fno-pie \
	-fno-stack-protector -msmall-data-limit=0 -ffunction-sections -fdata-sections \
	-fno-unwind-tables -fno-asynchronous-unwind-tables \
	-fno-tree-loop-distribute-patterns
LDFLAGS := -nostdlib -nostartfiles -static -Wl,--no-relax,--gc-sections
C_SOURCES := $(wildcard $(PROJECT_DIR)/*.c) $(SDK_DIR)/riscplay_audio.c
HEADERS := $(wildcard $(PROJECT_DIR)/*.h) $(wildcard $(SDK_DIR)/*.h)
ELF := $(BUILD_DIR)/bomberman.elf
BIN := $(BUILD_DIR)/bomberman.bin
PACKAGE := $(SERVER_BUILD_DIR)/bomberman.json.prog

.PHONY: all check-toolchain clean

all: $(PACKAGE)

check-toolchain:
	@command -v "$(CC)" >/dev/null 2>&1 || { echo "RISC-V GCC não encontrado: $(CC). Defina CROSS_COMPILE com o prefixo terminado em '-'." >&2; exit 1; }
	@command -v "$(OBJCOPY)" >/dev/null 2>&1 || { echo "objcopy RISC-V não encontrado: $(OBJCOPY)." >&2; exit 1; }
	@command -v python3 >/dev/null 2>&1 || { echo "python3 é necessário para gerar o pacote servido." >&2; exit 1; }

$(ELF): $(C_SOURCES) $(HEADERS) $(SDK_DIR)/startup.S $(SDK_DIR)/linker.ld | check-toolchain
	@mkdir -p "$(BUILD_DIR)"
	$(CC) $(ARCH_FLAGS) $(CFLAGS) -I"$(PROJECT_DIR)" -I"$(SDK_DIR)" \
		$(LDFLAGS) -Wl,-T,"$(SDK_DIR)/linker.ld" "$(SDK_DIR)/startup.S" \
		$(C_SOURCES) -lgcc -o "$@"

$(BIN): $(ELF)
	$(OBJCOPY) -O binary "$<" "$@"
	@size=$$(wc -c < "$@"); \
	if [ "$$size" -gt 28672 ]; then \
		echo "Firmware acima do limite de 28 KiB: $$size bytes." >&2; \
		exit 1; \
	fi

$(PACKAGE): $(BIN) | check-toolchain
	@mkdir -p "$(SERVER_BUILD_DIR)"
	python3 -c 'import json,pathlib,uuid; binary=pathlib.Path("$(BIN)").read_bytes(); package=[{"comp_uuid":str(uuid.uuid5(uuid.NAMESPACE_URL,"riscplay-studio/bomberman")),"type":0,"addr":"20000000","addr_mem":"20000000","value":binary.hex()}]; pathlib.Path("$@").write_text(json.dumps(package,separators=(",",":"))+"\n",encoding="utf-8")'
	@echo "Pacote disponível em /builds/bomberman.json.prog"

clean:
	rm -rf "$(BUILD_DIR)" "$(PACKAGE)"