#include "drivers/vga.h"
#include "kernel.h"
#include <stdbool.h>

static inline void hang(void);

void kpanic(const char* msg) {
	vga_color_set_bg(VGA_COLOR_BLACK);
	vga_color_set_fg(VGA_COLOR_LIGHT_GREY);
	vga_clear();

	vga_color_set_fg(VGA_COLOR_RED);
	vga_cursor_move(0, 0);
	vga_print("Kernel Panic!");

	vga_color_set_fg(VGA_COLOR_LIGHT_GREY);
	vga_cursor_move(0, 2);
	vga_print(msg);

	hang();
}

bool kinit(void) {
	bool sucess = true;

	if (!vga_init()) sucess = false;

	return sucess;
}

void kmain(unsigned long magic, unsigned long addr) {
       // Opcional: apenas para evitar aviso de variável não usada ai quando for mexer cuidado 
       (void)magic;
       (void)addr;

       if (!kinit()) {
               kpanic("Failed to initialize kernel!");
       }

       vga_print("hello, kernel!");

       while (1);
}

static inline void hang(void) {
	__asm__ volatile (
	    "cli\n\t"
	    "1:\n\t"
	    "hlt\n\t"
	    "jmp 1b\n\t"
	);
}
