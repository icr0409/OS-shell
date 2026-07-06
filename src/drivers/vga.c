#include "drivers/vga.h"
#include <stdint.h>

static volatile uint16_t* vga_buffer = (volatile uint16_t*)VGA_MEMORY;

static uint8_t vga_x = 0;
static uint8_t vga_y = 0;

static enum vga_color vga_fg = VGA_COLOR_LIGHT_GREY;
static enum vga_color vga_bg = VGA_COLOR_BLACK;

static void put_char(char c);

static uint16_t get_index(uint8_t x, uint8_t y);
static uint8_t get_color(enum vga_color fg, enum vga_color bg);
static uint16_t get_entry(uint8_t color, char c);

void vga_init(void) {
	vga_color_set_fg(VGA_COLOR_LIGHT_GREY);
	vga_color_set_bg(VGA_COLOR_BLACK);
	vga_clear();
}

void vga_print(const char* s) {
	uint16_t i = 0;

	while (s[i] != '\0') {
		char c = s[i];

		if (c == '\n' || vga_x >= VGA_WIDTH) {
			vga_cursor_move(0, ++vga_y);
			i++;
			continue;
		}

		if (vga_y >= VGA_HEIGHT) {
			vga_clear();
			vga_cursor_move(0, 0);
		}

		put_char(c);

		vga_x++;
		i++;
	}
}

void vga_clear(void) {
	for (uint8_t y = 0; y < VGA_HEIGHT; y++) {
		for (uint8_t x = 0; x < VGA_WIDTH; x++) {
			vga_cursor_move(x, y);
			put_char(' ');
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
}

uint8_t vga_cursor_get_x(void) {
	return vga_x;
}

uint8_t vga_cursor_get_y(void) {
	return vga_y;
}

void vga_cursor_show(void) {

}

void vga_cursor_hide(void) {

}

static void put_char(char c) {
	uint16_t index = get_index(vga_x, vga_y);
	uint8_t color = get_color(vga_fg, vga_bg);
	uint16_t entry = get_entry(color, c);

	vga_buffer[index] = entry;
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