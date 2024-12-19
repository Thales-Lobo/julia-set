#include <stdio.h>
#include <stdlib.h>
#include "append_to_csv.h"

/**
 * @file append_to_csv.c
 * @brief Appends execution data to a CSV file.
 *
 * This function appends a new row to a CSV file containing the number of processes,
 * the resolution, and the execution time of a program.  If the file does not exist,
 * it is created and a header row is added.
 *
 * @param file_path The path to the CSV file.
 * @param num_processes The number of processes used in the execution.
 * @param resolution The resolution setting used (meaning depends on the application).
 * @param execution_time The total execution time of the program in seconds.
 *
 * @return void
 *
 *
 * @note  The function handles file opening errors by printing an error message to stderr.
 *        It also ensures that the header row ("Processes,Resolution,ExecutionTime") is
 *        written only if the file is initially empty.  Execution time is written
 *        with 6 decimal places of precision.
 */
void append_to_csv(const char *file_path, int num_processes, int resolution, double execution_time) {
    FILE *file = fopen(file_path, "a");
    if (!file) {
        fprintf(stderr, "Error: Unable to open report file for writing.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(file, "Processes,Resolution,ExecutionTime\n");
    }

    fprintf(file, "%d,%d,%.6f\n", num_processes, resolution, execution_time);
    fclose(file);
}