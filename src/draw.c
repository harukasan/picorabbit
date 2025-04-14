#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "draw.h"
#include "framebuffer.h"

#include "../vendor/font8x8/font8x8_basic.h"

// Draw background
void draw_background(uint8_t *buffer, int color)
{
    memset(buffer, color, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT);
}

// Draw rectangle
void draw_rect(uint8_t *buffer, int x, int y, int width, int height, int color)
{
    x *= FRAMEBUFFER_PIXEL_WIDTH;
    y *= FRAMEBUFFER_PIXEL_HEIGHT;
    width *= FRAMEBUFFER_PIXEL_WIDTH;
    height *= FRAMEBUFFER_PIXEL_HEIGHT;

    // Boundary check
    if (x < 0 || y < 0 || x >= FRAMEBUFFER_WIDTH || y >= FRAMEBUFFER_HEIGHT)
    {
        return;
    }

    int draw_width = (x + width > FRAMEBUFFER_WIDTH) ? FRAMEBUFFER_WIDTH - x : width;
    int draw_height = (y + height > FRAMEBUFFER_HEIGHT) ? FRAMEBUFFER_HEIGHT - y : height;

    for (int j = 0; j < draw_height; j++)
    {
        memset(&buffer[(y + j) * FRAMEBUFFER_WIDTH + x], color, draw_width);
    }
}

// Draw text line
void draw_text(uint8_t *buffer, int x, int y, const char *text, int color)
{
    if (y + FONT_HEIGHT <= 0 || y >= FRAMEBUFFER_HEIGHT / FRAMEBUFFER_PIXEL_HEIGHT || x >= FRAMEBUFFER_WIDTH / FRAMEBUFFER_PIXEL_WIDTH)
    {
        return;
    }

    for (int t = 0; text[t] != '\0'; t++)
    {
        unsigned char c = (unsigned char)text[t];
        const uint8_t *glyph = font8x8_basic[c];
        if (!glyph)
            continue;

        int char_x = x + t * FONT_WIDTH;
        if (char_x + FONT_WIDTH <= 0 || char_x >= FRAMEBUFFER_WIDTH)
            continue;

        for (int j = 0; j < FONT_HEIGHT; j++)
        {
            int py = y + j;
            if (py < 0 || py >= FRAMEBUFFER_HEIGHT)
                continue;

            uint8_t bits = glyph[j];
            for (int i = 0; i < FONT_WIDTH; i++)
            {
                int px = (char_x + i) * FRAMEBUFFER_PIXEL_WIDTH;
                if (px < 0 || px >= FRAMEBUFFER_WIDTH)
                    continue;

                if (bits & (1 << i))
                {
                    buffer[py * FRAMEBUFFER_WIDTH + px] = color;
                    buffer[py * FRAMEBUFFER_WIDTH + px + 1] = color;
                }
            }
        }
    }
}

// FIXME: Apply for 2:1 scaling
// Draw text line faster (skips coordinate checks)
void draw_text_fast(uint8_t *buffer, int x, int y, const char *text, int color)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        const uint8_t *glyph = (uint8_t *)font8x8_basic[(uint8_t)text[i]];
        int base_x = (x + i * FONT_WIDTH) * FRAMEBUFFER_PIXEL_WIDTH;

        for (int dy = 0; dy < FONT_HEIGHT; dy++)
        {
            uint8_t bits = glyph[dy];
            uint8_t *dst = &buffer[(y + dy) * FRAMEBUFFER_WIDTH + base_x];

            for (int dx = 0; dx < FONT_WIDTH; dx++)
            {
                int px = dx * FRAMEBUFFER_PIXEL_WIDTH;
                if (bits & (1 << dx))
                {
                    dst[px] = color;
                    dst[px + 1] = color;
                }
            }
        }
    }
}

// Draw image data
void draw_image(uint8_t *buffer, const uint8_t *image_data, int x, int y, int image_width, int image_height) {
    x *= FRAMEBUFFER_PIXEL_WIDTH;
    y *= FRAMEBUFFER_PIXEL_HEIGHT;
    int output_width =  image_width * FRAMEBUFFER_PIXEL_WIDTH;
    int output_height = image_height * FRAMEBUFFER_PIXEL_HEIGHT;

    // Boundary check
    if (x < 0 || y < 0 || x >= FRAMEBUFFER_WIDTH || y >= FRAMEBUFFER_HEIGHT) {
        return;
    }

    int draw_width = (x + output_width > FRAMEBUFFER_WIDTH) ? FRAMEBUFFER_WIDTH - x : output_width;
    int draw_height = (y + output_height > FRAMEBUFFER_HEIGHT) ? FRAMEBUFFER_HEIGHT - y : output_height;

    for (int j = 0; j < draw_height; j++) {
        for (int i = 0; i < draw_width; i++) {
            int src_idx = (j / FRAMEBUFFER_PIXEL_HEIGHT) * image_width + (i / FRAMEBUFFER_PIXEL_WIDTH);
            int dst_idx = (y + j) * FRAMEBUFFER_WIDTH + (x + i);
            buffer[dst_idx] = image_data[src_idx];
        }
    }
}

// Draw image data with 1-bit per pixel transparency mask
void draw_image_masked(uint8_t *buffer, const uint8_t *image_data, const uint8_t *mask_data, int x, int y, int image_width, int image_height) {
    x *= FRAMEBUFFER_PIXEL_WIDTH;
    y *= FRAMEBUFFER_PIXEL_HEIGHT;
    int output_width = image_width * FRAMEBUFFER_PIXEL_WIDTH;
    int output_height = image_height * FRAMEBUFFER_PIXEL_HEIGHT;

    // Boundary check
    if (x < 0 || y < 0 || x >= FRAMEBUFFER_WIDTH || y >= FRAMEBUFFER_HEIGHT) {
        return;
    }

    int draw_width = (x + output_width > FRAMEBUFFER_WIDTH) ? FRAMEBUFFER_WIDTH - x : output_width;
    int draw_height = (y + output_height > FRAMEBUFFER_HEIGHT) ? FRAMEBUFFER_HEIGHT - y : output_height;

    for (int j = 0; j < draw_height; j++) {
        for (int i = 0; i < draw_width; i++) {
            int src_x = i / FRAMEBUFFER_PIXEL_WIDTH;
            int src_y = j / FRAMEBUFFER_PIXEL_HEIGHT;
            int src_idx = src_y * image_width + src_x;

            // Calculate mask byte index and bit position
            int mask_byte_idx = src_idx >> 3;    // Divide by 8 to get byte index
            int mask_bit_pos = src_idx & 0x07;   // Modulo 8 to get bit position

            // Skip pixel if it's transparent in the mask (bit = 0)
            if (!(mask_data[mask_byte_idx] & (1 << mask_bit_pos))) {
                continue;
            }

            int dst_idx = (y + j) * FRAMEBUFFER_WIDTH + (x + i);
            buffer[dst_idx] = image_data[src_idx];
        }
    }
}

