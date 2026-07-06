#pragma once

#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

enum vga_color {
	VGA_COLOR_BLACK 		= 0x0,
	VGA_COLOR_BLUE 			= 0x1,
	VGA_COLOR_GREEN 		= 0x2,
	VGA_COLOR_CYAN 			= 0x3,
	VGA_COLOR_RED 			= 0x4,
	VGA_COLOR_MAGENTA 		= 0x5,
	VGA_COLOR_BROWN 		= 0x6,
	VGA_COLOR_LIGHT_GREY 	= 0x7,
	VGA_COLOR_DARK_GREY 	= 0x8,
	VGA_COLOR_LIGHT_BLUE 	= 0x9,
	VGA_COLOR_LIGHT_GREEN 	= 0xA,
	VGA_COLOR_LIGHT_CYAN 	= 0xB,
	VGA_COLOR_LIGHT_RED     = 0xC,
	VGA_COLOR_LIGHT_MAGENTA = 0xD,
	VGA_COLOR_LIGHT_BROWN   = 0xE,
	VGA_COLOR_WHITE 		= 0xF,
};

void vga_init(void);

void vga_print(const char* string);
void vga_clear(void);

void vga_color_set_bg(enum vga_color color);
void vga_color_set_fg(enum vga_color color);
enum vga_color vga_color_get_bg(void);
enum vga_color vga_color_get_fg(void);

void vga_cursor_move(uint8_t x, uint8_t y);
uint8_t vga_cursor_get_x(void);
uint8_t vga_cursor_get_y(void);
void vga_cursor_enable(void);
void vga_cursor_disable(void);