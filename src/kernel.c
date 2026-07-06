#include "drivers/vga.h"

void kmain() {
	vga_init();
	vga_print("hello, kernel!");

	while (1);
}