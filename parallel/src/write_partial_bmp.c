#include <stdio.h>
#include "../include/write_partial_bmp.h"

/**
 * write_partial_bmp:
 * Writes a portion of a BMP image to a file, starting from a specific row.
 *
 * This function writes the pixel data for a given range of rows in a BMP image. 
 * It handles proper alignment by adding necessary padding to ensure each row 
 * size is a multiple of 4 bytes, as required by the BMP format.
 *
 * @param file: Pointer to the open BMP file in binary write mode.
 * @param start_row: The starting row (0-based) in the BMP image where writing begins.
 * @param rows: The number of rows to write.
 * @param width: The width of the image in pixels.
 * @param pixels: Pointer to the pixel data to be written. Each pixel is represented 
 *                as 3 bytes (RGB).
 *
 * @return: Returns 0 on success.
 */

int write_partial_bmp(FILE *file, int start_row, int rows, int width, unsigned char *pixels) {
    fseek(file, 54 + start_row * ((width * 3 + 3) & ~3), SEEK_SET);
    for (int y = 0; y < rows; y++) {
        fwrite(&pixels[y * 3 * width], 3, width, file);
        unsigned char padding[3] = {0, 0, 0};
        fwrite(padding, sizeof(char), (4 - (width * 3) % 4) % 4, file);
    }
    return 0;
}