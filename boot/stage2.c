// Stage 2 of Bootloader
// Version 1
// Author = AR-DEV-1
// This is licensed under GPL 3.0
#include "sys/include/stdint.h"
// VESA 1024x768, 16-bit color
#define VIDEO_MODE 0x118   
#define VESA_SET_MODE 0x4F02

typedef struct {
    uint16_t attributes;
    uint8_t win_a, win_b;
    uint16_t granularity;
    uint16_t win_size;
    uint16_t segment_a, segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;          // Bytes per scan line

    uint16_t width, height;  // Screen dimensions
    uint8_t w_char, y_char, planes;
    uint8_t bpp, banks;
    uint8_t memory_model, bank_size;
    uint8_t image_pages;
    uint8_t reserved;

    uint8_t red_mask_size, red_field_pos;
    uint8_t green_mask_size, green_field_pos;
    uint8_t blue_mask_size, blue_field_pos;
    uint8_t reserved_mask_size, reserved_field_pos;
    uint8_t direct_color_info;

    uint32_t phys_base;      // Physical memory address of framebuffer
    uint32_t offscreen_mem_offset;
    uint16_t offscreen_mem_size;
} __attribute__((packed)) vbe_mode_info_t;

void set_video_mode(uint16_t mode) {
    asm volatile (
        "int $0x10"
        :
        : "a"(VESA_SET_MODE), "b"(mode)
    );
}

void enter_long_mode() {
   
    // This function will handle switching from protected mode to long mode (64-bit mode)
    // The long mode transition involves several steps:
    // 1. Set up the GDT for 64-bit mode
    // 2. Set up the 64-bit code segment and jump to it

    
    // For simplicity, we're assuming the GDT and all necessary setups are already in place
 asm volatile (
    "mov eax, cr4\n"         ; Read control register 4
    "or eax, 0x20\n"         ; Set PAE (Physical Address Extension) bit
    "mov cr4, eax\n"         ; Write back to cr4
);

}

void long_mode_entry() {
    
    // 64-bit kernel entry code (can be extended with 64-bit setup)
    
    while (1) {}
}

void _start() {
    // Set the video mode to graphics
    
    set_video_mode(VIDEO_MODE);

    // Check if we're in 64-bit mode, and enter long mode
    
    enter_long_mode();
}
