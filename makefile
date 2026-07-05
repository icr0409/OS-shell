CC = gcc
AS = nasm
LD = ld

GRUB_MKRESCUE = grub-mkrescue

SRC_DIR    = src
GRUB_DIR   = grub
BUILD_DIR  = build
ISO_DIR    = $(BUILD_DIR)/isodir

BOOT_OBJ   = $(BUILD_DIR)/boot.asm.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.c.o
OBJ = \
	$(BOOT_OBJ) \
	$(KERNEL_OBJ)

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
	-fno-stack-protector

ASFLAGS = \
	-f elf32

LDFLAGS = \
	-m elf_i386 \
	-T $(LINKER)

.PHONY: all clean run iso

all: $(ISO)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_OBJ): $(SRC_DIR)/boot.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL_OBJ): $(SRC_DIR)/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(OBJ) $(LINKER)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

$(ISO): $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/os-shell.bin
	cp $(GRUB_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

iso: $(ISO)

run: $(ISO)
	$(QEMU) -cdrom $(ISO)

clean:
	rm -rf $(BUILD_DIR)