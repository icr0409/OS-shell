CC = gcc
AS = nasm
LD = ld
GRUB_MKRESCUE = grub-mkrescue

SRC_DIR    = src
INC_DIR    = include
GRUB_DIR   = grub
BUILD_DIR  = build
ISO_DIR    = $(BUILD_DIR)/isodir

C_SRCS   = $(shell find $(SRC_DIR) -name '*.c')
ASM_SRCS = $(shell find $(SRC_DIR) -name '*.asm')

C_OBJS   = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.c.o,$(C_SRCS))
ASM_OBJS = $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.asm.o,$(ASM_SRCS))

OBJ = $(ASM_OBJS) $(C_OBJS)

LINKER     = $(SRC_DIR)/linker.ld
KERNEL_BIN = $(BUILD_DIR)/os-shell.bin
ISO        = $(BUILD_DIR)/os-shell.iso

QEMU = qemu-system-i386
QEMU_FLAGS = \
	-cdrom $(ISO)

CFLAGS = \
	-m32 \
	-ffreestanding \
	-O2 \
	-Wall \
	-Wextra \
	-fno-exceptions \
	-fno-stack-protector \
	-I$(INC_DIR)

ASFLAGS = \
	-f elf32 \
	-i $(INC_DIR)/

LDFLAGS = \
	-m elf_i386 \
	-T $(LINKER)

.PHONY: all clean run iso

all: $(ISO)


$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL_BIN): $(OBJ) $(LINKER)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

$(ISO): $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/os-shell.bin
	cp $(GRUB_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

iso: $(ISO)

run: $(ISO)
	$(QEMU) $(QEMU_FLAGS)

clean:
	rm -rf $(BUILD_DIR)