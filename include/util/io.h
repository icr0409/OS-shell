#pragma once

#include <stdint.h>

static inline void io_outb(uint8_t value, uint16_t port) {
	asm volatile("outb %0, %1" : : "a"(value), "d"(port));
}

static inline uint8_t io_inb(uint16_t port) {
	uint8_t data;
    asm volatile("inb %1, %0" : "=a"(data) : "d"(port));
    return data;
}