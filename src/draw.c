#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

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
void draw_image(uint8_t *buffer, const uint8_t *image_data, int x, int y, int image_width, int image_height)
{
    x *= FRAMEBUFFER_PIXEL_WIDTH;
    y *= FRAMEBUFFER_PIXEL_HEIGHT;
    int output_width = image_width * FRAMEBUFFER_PIXEL_WIDTH;
    int output_height = image_height * FRAMEBUFFER_PIXEL_HEIGHT;

    // Boundary check
    if (x < 0 || y < 0 || x >= FRAMEBUFFER_WIDTH || y >= FRAMEBUFFER_HEIGHT)
    {
        return;
    }

    int draw_width = (x + output_width > FRAMEBUFFER_WIDTH) ? FRAMEBUFFER_WIDTH - x : output_width;
    int draw_height = (y + output_height > FRAMEBUFFER_HEIGHT) ? FRAMEBUFFER_HEIGHT - y : output_height;

    for (int j = 0; j < draw_height; j++)
    {
        for (int i = 0; i < draw_width; i++)
        {
            int src_idx = (j / FRAMEBUFFER_PIXEL_HEIGHT) * image_width + (i / FRAMEBUFFER_PIXEL_WIDTH);
            int dst_idx = (y + j) * FRAMEBUFFER_WIDTH + (x + i);
            buffer[dst_idx] = image_data[src_idx];
        }
    }
}

// Draw image data with 1-bit per pixel transparency mask
void draw_image_masked(uint8_t *buffer, const uint8_t *image_data, const uint8_t *mask_data, int x, int y, int image_width, int image_height)
{
    x *= FRAMEBUFFER_PIXEL_WIDTH;
    y *= FRAMEBUFFER_PIXEL_HEIGHT;
    int output_width = image_width * FRAMEBUFFER_PIXEL_WIDTH;
    int output_height = image_height * FRAMEBUFFER_PIXEL_HEIGHT;

    // Boundary check
    if (x < 0 || y < 0 || x >= FRAMEBUFFER_WIDTH || y >= FRAMEBUFFER_HEIGHT)
    {
        return;
    }

    int draw_width = (x + output_width > FRAMEBUFFER_WIDTH) ? FRAMEBUFFER_WIDTH - x : output_width;
    int draw_height = (y + output_height > FRAMEBUFFER_HEIGHT) ? FRAMEBUFFER_HEIGHT - y : output_height;

    for (int j = 0; j < draw_height; j++)
    {
        for (int i = 0; i < draw_width; i++)
        {
            int src_x = i / FRAMEBUFFER_PIXEL_WIDTH;
            int src_y = j / FRAMEBUFFER_PIXEL_HEIGHT;
            int src_idx = src_y * image_width + src_x;

            // Calculate mask byte index and bit position
            int mask_byte_idx = src_idx >> 3;  // Divide by 8 to get byte index
            int mask_bit_pos = src_idx & 0x07; // Modulo 8 to get bit position

            // Skip pixel if it's transparent in the mask (bit = 0)
            if (!(mask_data[mask_byte_idx] & (1 << mask_bit_pos)))
            {
                continue;
            }

            int dst_idx = (y + j) * FRAMEBUFFER_WIDTH + (x + i);
            buffer[dst_idx] = image_data[src_idx];
        }
    }
}

// Draw image data with 1-bit per pixel transparency mask and rotation
void draw_image_masked_rotated(uint8_t *buffer, const uint8_t *image_data, const uint8_t *mask_data,
                               int x, int y, int image_width, int image_height, float angle_degrees)
{
    x *= FRAMEBUFFER_PIXEL_WIDTH;
    y *= FRAMEBUFFER_PIXEL_HEIGHT;
    int base_width = image_width * FRAMEBUFFER_PIXEL_WIDTH;
    int base_height = image_height * FRAMEBUFFER_PIXEL_HEIGHT;

    // Convert angle to radians
    float angle_rad = angle_degrees * 3.14159f / 180.0f;
    float sin_angle = sinf(angle_rad);
    float cos_angle = cosf(angle_rad);

    // Calculate rotated dimensions for drawing area
    int output_width = (int)((fabsf(base_width * cos_angle) + fabsf(base_height * sin_angle)) * FRAMEBUFFER_PIXEL_WIDTH) + 10;
    int output_height = (int)((fabsf(base_width * sin_angle) + fabsf(base_height * cos_angle)) * FRAMEBUFFER_PIXEL_HEIGHT) + 10;

    // Adjust for pixel aspect ratio (2:1)
    float aspect_ratio = (float)FRAMEBUFFER_PIXEL_WIDTH / FRAMEBUFFER_PIXEL_HEIGHT;

    // Calculate center points using original image dimensions
    int center_x = x + base_width / 2;
    int center_y = y + base_height / 2;

    // For each pixel in the destination space
    for (int dy = -output_height / 2; dy < output_height / 2; dy++)
    {
        for (int dx = -output_width / 2; dx < output_width / 2; dx++)
        {
            // Adjust coordinates for aspect ratio before rotation
            float adjusted_dx = (float)dx / aspect_ratio;

            // Rotate the point around the center
            float rotated_x = (adjusted_dx * cos_angle - dy * sin_angle) * aspect_ratio;
            float rotated_y = adjusted_dx * sin_angle + dy * cos_angle;

            // Convert back to source image coordinates
            int src_x = (int)((rotated_x / FRAMEBUFFER_PIXEL_WIDTH) + image_width / 2);
            int src_y = (int)((rotated_y / FRAMEBUFFER_PIXEL_HEIGHT) + image_height / 2);

            // Check if the source pixel is within the image bounds
            if (src_x >= 0 && src_x < image_width && src_y >= 0 && src_y < image_height)
            {
                int src_idx = src_y * image_width + src_x;
                int mask_byte_idx = src_idx >> 3;
                int mask_bit_pos = src_idx & 0x07;

                // Skip pixel if it's transparent in the mask (bit = 0)
                if (!(mask_data[mask_byte_idx] & (1 << mask_bit_pos)))
                {
                    continue;
                }

                // Calculate destination coordinates
                int dest_x = center_x + dx;
                int dest_y = center_y + dy;

                // Check if the destination pixel is within the framebuffer bounds
                if (dest_x >= 0 && dest_x < FRAMEBUFFER_WIDTH &&
                    dest_y >= 0 && dest_y < FRAMEBUFFER_HEIGHT)
                {
                    buffer[dest_y * FRAMEBUFFER_WIDTH + dest_x] = image_data[src_idx];
                }
            }
        }
    }
}

// Draw line using Bresenham's algorithm
void draw_line(uint8_t *buffer, int x0, int y0, int x1, int y1, int color)
{
    x0 *= FRAMEBUFFER_PIXEL_WIDTH;
    y0 *= FRAMEBUFFER_PIXEL_HEIGHT;
    x1 *= FRAMEBUFFER_PIXEL_WIDTH;
    y1 *= FRAMEBUFFER_PIXEL_HEIGHT;

    // Boundary check
    if (x0 < 0 || y0 < 0 || x0 >= FRAMEBUFFER_WIDTH || y0 >= FRAMEBUFFER_HEIGHT ||
        x1 < 0 || y1 < 0 || x1 >= FRAMEBUFFER_WIDTH || y1 >= FRAMEBUFFER_HEIGHT)
    {
        return;
    }

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    while (1)
    {
        // Draw pixel at current position
        if (x0 >= 0 && x0 < FRAMEBUFFER_WIDTH && y0 >= 0 && y0 < FRAMEBUFFER_HEIGHT)
        {
            buffer[y0 * FRAMEBUFFER_WIDTH + x0] = color;
            // Apply pixel scaling
            if (FRAMEBUFFER_PIXEL_WIDTH == 2)
            {
                buffer[y0 * FRAMEBUFFER_WIDTH + x0 + 1] = color;
            }
        }

        if (x0 == x1 && y0 == y1)
            break;

        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

// Draw text with escape sequence for color (\e[xx] for color, \e[reset] for default)
void draw_text_esc(uint8_t *buffer, int x, int y, const char *text, int default_color)
{
    int color = default_color;
    int px = x;
    int i = 0, seg_start = 0;
    while (text[i] != '\0')
    {
        if (text[i] == '\\' && text[i + 1] == 'e' && text[i + 2] == '[')
        {
            // 直前までの部分文字列を描画
            if (i > seg_start)
            {
                int len = i - seg_start;
                char tmp[256];
                if (len > 0 && len < (int)sizeof(tmp))
                {
                    memcpy(tmp, &text[seg_start], len);
                    tmp[len] = '\0';
                    draw_text(buffer, px, y, tmp, color);
                    px += len * FONT_WIDTH;
                }
            }
            int j = i + 3;
            if (text[j] == 'r' && strncmp(&text[j], "reset]", 6) == 0)
            {
                color = default_color;
                i = j + 6;
                seg_start = i;
                continue;
            }
            else if (isxdigit(text[j]) && isxdigit(text[j + 1]) && text[j + 2] == ']')
            {
                char hex[3] = {text[j], text[j + 1], '\0'};
                color = (int)strtol(hex, NULL, 16);
                i = j + 3;
                seg_start = i;
                continue;
            }
        }
        i++;
    }
    // 残りの部分文字列を描画
    if (i > seg_start)
    {
        int len = i - seg_start;
        char tmp[256];
        if (len > 0 && len < (int)sizeof(tmp))
        {
            memcpy(tmp, &text[seg_start], len);
            tmp[len] = '\0';
            draw_text(buffer, px, y, tmp, color);
        }
    }
}
