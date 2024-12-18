#include <stdio.h>
#include <math.h>
#include "../include/compute_julia_pixel.h"

/*
 * compute_julia_pixel(): calcula os valores RGB de um pixel em
 *                        uma imagem específica de um conjunto de Julia.
 *
 *  Entrada:
 *      (x, y):           coordenadas do pixel
 *      (width, height):  dimensões da imagem
 *      tint_bias:        um valor float para ajustar a tonalidade (1.0 é "sem ajuste")
 *  Saída:
 *      rgb: um array já alocado de 3 bytes onde serão escritos os valores
 *           R, G e B.
 *
 *  Retorno:
 *      0 em caso de sucesso, -1 em caso de falha
 *
 */

int compute_julia_pixel(int x, int y, int width, int height, float tint_bias, unsigned char *rgb) {

  // Verifica se as coordenadas são válidas
  if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
    fprintf(stderr, "Coordenadas inválidas (%d,%d) para um pixel em uma imagem de %d x %d\n", x, y, width, height);
    return -1;
  }

  // "Amplia" a visualização para mostrar uma área agradável do conjunto de Julia
  float X_MIN = -1.6, X_MAX = 1.6, Y_MIN = -0.9, Y_MAX = +0.9;
  float float_y = (Y_MAX - Y_MIN) * (float)y / height + Y_MIN;
  float float_x = (X_MAX - X_MIN) * (float)x / width + X_MIN;

  // Ponto que define o conjunto de Julia
  float julia_real = -.79;
  float julia_img = .15;

  // Número máximo de iterações
  int max_iter = 300;

  // Calcula a convergência da série complexa
  float real = float_y, img = float_x;
  int num_iter = max_iter;
  while ((img * img + real * real < 2 * 2) && (num_iter > 0)) {
    float xtemp = img * img - real * real + julia_real;
    real = 2 * img * real + julia_img;
    img = xtemp;
    num_iter--;
  }

  // Pinta o pixel com base no número de iterações usando uma coloração estilizada
  float color_bias = (float) num_iter / max_iter;
  rgb[0] = (num_iter == 0 ? 200 : -500.0 * pow(tint_bias, 1.2) * pow(color_bias, 1.6));
  rgb[1] = (num_iter == 0 ? 100 : -255.0 * pow(color_bias, 0.3));
  rgb[2] = (num_iter == 0 ? 100 : 255 - 255.0 * pow(tint_bias, 1.2) * pow(color_bias, 3.0));

  return 0;
}
