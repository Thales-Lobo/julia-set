#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "../include/compute_julia_pixel.h"
#include "../include/display_progress.h"
#include "../include/save_image_to_bmp.h"
#include "../include/append_to_csv.h"

#define OUTPUT_IMAGE_PATH "images/julia.bmp"
#define REPORT_PATH "report/sequential_execution_report.csv"

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

    clock_t start_time = clock();

    unsigned char *pixels = (unsigned char *)malloc(3 * width * height * sizeof(unsigned char));
    if (!pixels) {
        fprintf(stderr, "Error: Unable to allocate memory for pixel array.\n");
        return EXIT_FAILURE;
    }

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
        display_progress((y + 1) * width, total_pixels);
    }
    printf("\n");

    printf("Saving image to '%s'...\n", OUTPUT_IMAGE_PATH);
    if (save_image_to_bmp(OUTPUT_IMAGE_PATH, width, height, pixels) != 0) {
        fprintf(stderr, "Error: Failed to save image to BMP file.\n");
        free(pixels);
        return EXIT_FAILURE;
    }

    clock_t end_time = clock();
    double total_execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Execution completed in %.6f seconds.\n", total_execution_time);

    append_to_csv(REPORT_PATH, 1, n, total_execution_time);

    free(pixels);

    return EXIT_SUCCESS;
}
