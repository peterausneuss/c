#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h> // For fb_var_screeninfo, fb_fix_screeninfo

// Basic error handling macro
#define PERROR_EXIT(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Simple function to set a pixel (assumes 32-bit color depth for simplicity)
void set_pixel(char *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo,
               int x, int y, unsigned int color) {
    if (x < 0 || x >= vinfo->xres || y < 0 || y >= vinfo->yres) {
        //fprintf(stderr, "Pixel (%d,%d) out of bounds\n", x, y);
        return;
    }

    long location = (x + vinfo->xoffset) * (vinfo->bits_per_pixel / 8) +
                    (y + vinfo->yoffset) * finfo->line_length;

    // Assuming 32 bpp (e.g., RGBA or ARGB). Byte order might matter.
    *((unsigned int *)(fbp + location)) = color;
}

// --- This is a placeholder for a very simple font ---
// For a real application, you'd load or embed a proper font.
// This example won't actually draw "hello" but shows where font logic would go.
// Each char would be a small bitmap, e.g., 8x8 pixels.
// For 'H':
// XXXXXXX
// X  X  X
// X  X  X
// XXXXXXX
// X  X  X
// X  X  X
// XXXXXXX

void draw_char_H_at(char *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo,
                    int start_x, int start_y, unsigned int color) {
    // Example: Draw a very crude 'H' (just a few points for illustration)
    // A real implementation would iterate over a bitmap font for the character.
    for (int i = 0; i < 7; ++i) { // Vertical line
        set_pixel(fbp, vinfo, finfo, start_x, start_y + i, color);
        set_pixel(fbp, vinfo, finfo, start_x + 4, start_y + i, color);
    }
    for (int i = 0; i < 5; ++i) { // Horizontal line
        set_pixel(fbp, vinfo, finfo, start_x + i, start_y + 3, color);
    }
}


int main() {
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0; // Framebuffer pointer

    // 1. Open the framebuffer device
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        PERROR_EXIT("Error: cannot open framebuffer device. (Are you root? Is /dev/fb0 present?)");
    }
    printf("Framebuffer device opened successfully.\n");

    // 2. Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        PERROR_EXIT("Error reading fixed information");
    }

    // 3. Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        PERROR_EXIT("Error reading variable information");
    }

    printf("Screen resolution: %dx%d, %d bpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Calculate screen size in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    // Or more accurately using line_length for pitch:
    // screensize = vinfo.yres_virtual * finfo.line_length;


    // 4. Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((intptr_t)fbp == -1) {
        PERROR_EXIT("Error: failed to map framebuffer device to memory");
    }
    printf("Framebuffer mapped to memory at %p.\n", fbp);

    // --- Now you can write to fbp ---
    // Example: Draw a white 'H' near the top-left (10,10)
    // For "hello", you would loop through characters, get their font data,
    // and call set_pixel (or a character drawing function) repeatedly.
    
    // Clear a small portion of the screen to black (optional, for visibility)
    // for (int y = 0; y < 50; y++) {
    //     for (int x = 0; x < 200; x++) {
    //         set_pixel(fbp, &vinfo, &finfo, x, y, 0x00000000); // Black
    //     }
    // }

    unsigned int white_color = 0x00FFFFFF; // Assuming 32bpp, format might be ARGB or XRGB
                                         // For 16bpp (565), it would be different.
    
    printf("Attempting to draw...\n");
    // This is where you'd implement logic to draw "hello"
    // For example, to draw a very crude 'H':
    draw_char_H_at(fbp, &vinfo, &finfo, 20, 20, white_color);
    // To draw "hello", you'd need a font and a loop:
    // draw_string("hello", fbp, &vinfo, &finfo, start_x, start_y, color);

    // Add a small delay so it's visible if run from a virtual console
    // that might switch back too quickly.
    sleep(5);


    // 5. Unmap the framebuffer and close the file
    munmap(fbp, screensize);
    close(fbfd);

    printf("Framebuffer unmapped and closed.\n");
    return 0;
}
