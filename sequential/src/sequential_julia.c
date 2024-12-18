#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../include/compute_julia_pixel.h"
#include "../include/display_progress.h"
#include "../include/save_image_to_bmp.h"

// Define the output path as a constant
#define OUTPUT_IMAGE_PATH "images/julia.bmp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Error: <n> must be a strictly positive integer.\n");
        return EXIT_FAILURE;
    }

    int width = 2 * n;
    int height = n;

    printf("Generating Julia set image of size %dx%d...\n", width, height);

    unsigned char *pixels = (unsigned char *)malloc(3 * width * height * sizeof(unsigned char));
    if (!pixels) {
        fprintf(stderr, "Error: Unable to allocate memory for pixel array.\n");
        return EXIT_FAILURE;
    }

    // Generate the pixel data and update progress
    int total_pixels = width * height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char rgb[3];
            if (compute_julia_pixel(x, y, width, height, 1.0, rgb) == 0) {
                int index = (y * width + x) * 3;
                pixels[index] = rgb[0];
                pixels[index + 1] = rgb[1];
                pixels[index + 2] = rgb[2];
            }
        }
        // Update progress for each row
        display_progress((y + 1) * width, total_pixels);
    }
    printf("\n");

    // Save the image to a BMP file
    printf("Saving image to '%s'...\n", OUTPUT_IMAGE_PATH);
    if (save_image_to_bmp(OUTPUT_IMAGE_PATH, width, height, pixels) != 0) {
        fprintf(stderr, "Error: Failed to save image to BMP file.\n");
        free(pixels);
        return EXIT_FAILURE;
    }

    free(pixels);

    return EXIT_SUCCESS;
}
