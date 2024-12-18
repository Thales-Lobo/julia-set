#ifndef DISPLAY_PROGRESS_H
#define DISPLAY_PROGRESS_H

#include <stdbool.h>
#include <time.h>

void display_progress_full(int current, int total, int bar_width, int precision, bool show_time);

void display_progress(int current, int total);

#endif // DISPLAY_PROGRESS_H
