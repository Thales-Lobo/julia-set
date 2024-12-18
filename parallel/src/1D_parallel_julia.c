#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/compute_julia_pixel.h"
#include "../include/write_bmp_header.h"
#include "../include/write_partial_bmp.h"

#define OUTPUT_IMAGE_PATH "images/julia_mpi.bmp"

int main(int argc, char *argv[]) {
    int rank, size, n;

    MPI_Init(&argc, &argv);
    double total_start_time = MPI_Wtime();
    double setup_start_time, setup_end_time, barrier_time = 0, finalize_time;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Medir tempo de setup (argumentos e inicializações)
    setup_start_time = MPI_Wtime();
    if (argc != 2) {
        if (rank == 0) fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    n = atoi(argv[1]);
    if (n <= 0) {
        if (rank == 0) fprintf(stderr, "Error: <n> must be a positive integer.\n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int width = 2 * n, height = n;
    int rows_per_process = height / size;
    int remaining_rows = height % size;

    // Determinar o intervalo de linhas de cada processo
    int start_row = rank * rows_per_process + (rank < remaining_rows ? rank : remaining_rows);
    int local_rows = rows_per_process + (rank < remaining_rows ? 1 : 0);

    // Alocar espaço para os pixels locais
    unsigned char *pixels = (unsigned char *)malloc(3 * width * local_rows * sizeof(unsigned char));
    if (!pixels) {
        fprintf(stderr, "[Process %d] Memory allocation failed.\n", rank);
        MPI_Finalize();
        return EXIT_FAILURE;
    }
    setup_end_time = MPI_Wtime();

    // Computar os pixels
    double compute_start_time = MPI_Wtime();
    for (int y = 0; y < local_rows; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char rgb[3];
            compute_julia_pixel(x, start_row + y, width, height, 1.0, rgb);
            int idx = (y * width + x) * 3;
            pixels[idx] = rgb[0];
            pixels[idx + 1] = rgb[1];
            pixels[idx + 2] = rgb[2];
        }
    }
    double compute_end_time = MPI_Wtime();

    // Processo 0 cria o arquivo e escreve o cabeçalho
    double header_write_time = 0;
    if (rank == 0) {
        double header_write_start_time = MPI_Wtime();
        FILE *file = fopen(OUTPUT_IMAGE_PATH, "wb");
        if (!file) {
            fprintf(stderr, "Error: Unable to open file for writing.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        if (write_bmp_header(file, width, height) != 0) {
            fprintf(stderr, "Error: Failed to write BMP header.\n");
            fclose(file);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        fclose(file);
        double header_write_end_time = MPI_Wtime();
        header_write_time = header_write_end_time - header_write_start_time;
    }

    // Sincronização explícita para escrita no arquivo
    double barrier_start_time = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    barrier_time += MPI_Wtime() - barrier_start_time;

    if (rank > 0) {
        MPI_Recv(NULL, 0, MPI_BYTE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Escrita das linhas no arquivo
    double file_write_start_time = MPI_Wtime();
    FILE *file = fopen(OUTPUT_IMAGE_PATH, "r+b");
    if (!file) {
        fprintf(stderr, "[Process %d] Error opening file.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    write_partial_bmp(file, start_row, local_rows, width, pixels);
    fclose(file);
    double file_write_end_time = MPI_Wtime();

    if (rank < size - 1) {
        MPI_Send(NULL, 0, MPI_BYTE, rank + 1, 0, MPI_COMM_WORLD);
    }

    // Medir tempo de finalização
    finalize_time = MPI_Wtime();
    double total_end_time = finalize_time;

    // Exibir relatório
    double total_execution_time = total_end_time - total_start_time;
    double file_write_time = file_write_end_time - file_write_start_time;

    printf("[Process %d] Timing Report [%d-%d]:\n", rank, start_row, start_row + local_rows - 1);
    printf("  Setup: %.3f seconds\n", setup_end_time - setup_start_time);
    printf("  Computation: %.3f seconds\n", compute_end_time - compute_start_time);
    printf("  Header Writing: %.3f seconds\n", header_write_time);
    printf("  File Writing: %.3f seconds\n", file_write_time);
    printf("  Barrier Synchronization: %.3f seconds\n", barrier_time);
    printf("  Total Execution: %.3f seconds\n", total_execution_time);

    free(pixels);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
