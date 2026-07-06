#include "drivers/vga.h"
#include "util/io.h"
#include <stdint.h>

static volatile uint16_t* vga_buffer = (volatile uint16_t*)VGA_MEMORY;

static uint8_t vga_x = 0;
static uint8_t vga_y = 0;

static enum vga_color vga_fg = VGA_COLOR_LIGHT_GREY;
static enum vga_color vga_bg = VGA_COLOR_BLACK;

static uint8_t cursor_start;

static void put_char(char c);

static uint16_t get_index(uint8_t x, uint8_t y);
static uint8_t get_color(enum vga_color fg, enum vga_color bg);
static uint16_t get_entry(uint8_t color, char c);

bool vga_init(void) {
	vga_color_set_fg(VGA_COLOR_LIGHT_GREY);
	vga_color_set_bg(VGA_COLOR_BLACK);
	vga_cursor_enable();
	vga_clear();

    return true;
}

void vga_print(const char* s) {
    for (uint16_t i = 0; s[i] != '\0'; i++) {
        char c = s[i];

        if (c == '\n') {
            vga_x = 0;
            vga_y++;
        } else {
            put_char(c);
            vga_x++;
        }

        if (vga_x >= VGA_WIDTH) {
            vga_x = 0;
            vga_y++;
        }

        if (vga_y >= VGA_HEIGHT) {
            vga_clear();
            vga_x = 0;
            vga_y = 0;
        }
    }

    vga_cursor_move(vga_x, vga_y);
}

void vga_clear(void) {
    for (uint8_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint8_t x = 0; x < VGA_WIDTH; x++) {
            uint16_t index = get_index(x, y);
            vga_buffer[index] = get_entry(get_color(vga_fg, vga_bg), ' ');
        }
    }

    vga_cursor_move(0, 0);
}

void vga_color_set_fg(enum vga_color color) {
	vga_fg = color;
}

void vga_color_set_bg(enum vga_color color) {
	vga_bg = color;
}

enum vga_color vga_color_get_fg(void) {
	return vga_fg;
}

enum vga_color vga_color_get_bg(void) {
	return vga_bg;
}

void vga_cursor_move(uint8_t x, uint8_t y) {
	vga_x = x;
	vga_y = y;

	uint16_t pos = get_index(x, y);

	io_outb(0x0F, 0x3D4);
    io_outb((uint8_t)(pos & 0xFF), 0x3D5);

    io_outb(0x0E, 0x3D4);
    io_outb((uint8_t)(pos >> 8), 0x3D5);
}

uint8_t vga_cursor_get_x(void) {
	return vga_x;
}

uint8_t vga_cursor_get_y(void) {
	return vga_y;
}

void vga_cursor_enable(void) {
    io_outb(0x0A, 0x3D4);
    cursor_start = io_inb(0x3D5) & 0x1F;
    io_outb(cursor_start, 0x3D5);
}

void vga_cursor_disable(void) {
	io_outb(0x0A, 0x3D4);
    io_outb(0x20, 0x3D5);
}

static void put_char(char c) {
    uint16_t index = get_index(vga_x, vga_y);
    uint8_t color = get_color(vga_fg, vga_bg);

    vga_buffer[index] = get_entry(color, c);
}

static uint16_t get_index(uint8_t x, uint8_t y) {
	return y * VGA_WIDTH + x;
}

static uint8_t get_color(enum vga_color fg, enum vga_color bg) {
	return ((uint8_t)bg << 4 | (uint8_t) fg);
}

static uint16_t get_entry(uint8_t color, char c) {
	return ((uint16_t)color << 8) | (uint16_t)c;
}