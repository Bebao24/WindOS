TARGET_ASMFLAGS += -f elf64
TARGET_CFLAGS += -Wall -Wextra -ffreestanding -fno-stack-protector -fno-stack-check \
				-mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -fPIE -mcmodel=large

TARGET_LINKFLAGS += -m elf_x86_64 \
				   -nostdlib \
				   -static \
				   -z max-page-size=0x1000 \
				   -T linker.ld

TARGET_CFLAGS += -I ../lib -I ../lib/boot -I src -I src/arch/x86_64 -I src/arch/x86_64/cpu -I src/debug \
				-I src/drivers -I src/graphics

C_SOURCES := $(shell find src -name "*.c")
C_OBJECTS := $(patsubst src/%.c, $(BUILD_DIR)/kernel/x86_64/%.o, $(C_SOURCES))

C_HEADERS := $(shell find src -name "*.h")

TARGET_DIR = ../target

$(TARGET_DIR)/kernel/kernel.bin: $(C_OBJECTS)
	@ mkdir -p $(TARGET_DIR)/kernel
	$(TARGET_LD) $(TARGET_LINKFLAGS) -o $(BUILD_DIR)/kernel/kernel.bin $^ $(TARGET_LIBS)
	cp $(BUILD_DIR)/kernel/kernel.bin $@

$(BUILD_DIR)/kernel/x86_64/%.o: src/%.c $(C_HEADERS)
	@ mkdir -p $(@D)
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<

