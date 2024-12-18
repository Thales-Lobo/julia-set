#include "display_progress.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Static variable to store start time
static clock_t start_time = 0;

/**
 * display_progress_full:
 * Displays a dynamic progress bar on the terminal and, when complete, prints
 * the total execution time in milliseconds.
 *
 * @param rank: Rank of the MPI process.
 * @param current: Current progress.
 * @param total: Total amount to be reached.
 * @param bar_width: Width of the progress bar (default is 50).
 * @param precision: Number of decimal places for the elapsed time (default is 2, max is 5).
 * @param show_time: Whether to display execution time at 100% (default is true).
 */
void display_progress_full(int rank, int current, int total, int bar_width, int precision, bool show_time) {
    // Default values for optional parameters
    if (bar_width <= 0) bar_width = 50;
    if (precision < 0 || precision > 5) precision = 2;

    // Record the start time on first call
    if (start_time == 0) {
        start_time = clock();
    }

    // Ensure progress doesn't exceed total
    if (current > total) {
        current = total;
    }

    // Calculate progress ratio
    float progress = (float)current / total;
    int position = (int)(bar_width * progress);

    // Print progress bar
    printf("\r[Process %d] [", rank);
    for (int i = 0; i < bar_width; ++i) {
        if (i <= position) {
            printf("=");
        } else {
            printf(" ");
        }
    }

    // Print percentage
    printf("] %d%%", (int)(progress * 100));

    // If 100% reached, print elapsed time
    if (current == total && show_time) {
        clock_t end_time = clock();
        double elapsed_ms = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;

        // Format string for precision
        char format_str[16];
        snprintf(format_str, sizeof(format_str), " (%%.%df ms)", precision);

        printf(format_str, elapsed_ms);
    }

    fflush(stdout);
}

/**
 * Wrapper function with default values.
 */
void display_progress(int rank, int current, int total) {
    int BAR_WIDTH = 50;
    int PRECISION = 2;
    bool SHOW_TIME = false;

    display_progress_full(rank, current, total, BAR_WIDTH, PRECISION, SHOW_TIME);
}