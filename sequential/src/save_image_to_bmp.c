#include <stdio.h>
#include "../include/write_bmp_header.h"
#include "../include/save_image_to_bmp.h"

/**
 * save_image_to_bmp:
 * Saves a pixel array as a BMP file.
 *
 * @param filename: Name of the BMP file to save.
 * @param width: Image width in pixels.
 * @param height: Image height in pixels.
 * @param pixels: 1D array of RGB pixel values (3 bytes per pixel).
 * @return: 0 on success, -1 on failure.
 */
int save_image_to_bmp(const char *filename, int width, int height, unsigned char *pixels) {
    FILE *output_file = fopen(filename, "wb");
    if (!output_file) {
        fprintf(stderr, "Error: Unable to create file %s\n", filename);
        return -1;
    }

    if (write_bmp_header(output_file, width, height) != 0) {
        fprintf(stderr, "Error: Failed to write BMP header.\n");
        fclose(output_file);
        return -1;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fwrite(&(pixels[y * 3 * width + x * 3]), sizeof(char), 3, output_file);
        }

        unsigned char padding[3] = {0, 0, 0};
        fwrite(padding, sizeof(char), (4 - (width * 3) % 4) % 4, output_file);
    }

    fseek(output_file, 0, SEEK_END);
    long file_size = ftell(output_file);

    float file_size_mb = (float)file_size / (1024 * 1024);

    fclose(output_file);

    printf("Julia set successfully saved to '%s' [%.2f MB].\n", filename, file_size_mb);

    return 0;
}