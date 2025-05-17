#ifndef HELPER_H
#define HELPER_H

#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "algoritms.h"
#include "gui.h"

#ifdef __APPLE__
#include <gtkmacintegration/gtkosxapplication.h>
#endif

#define MAX_ARRAY_SIZE 200
#define MIN_ARRAY_SIZE ((MAX_ARRAY_SIZE / 2) + (MAX_ARRAY_SIZE / 4))
#define MIN_VALUE -MAX_ARRAY_SIZE
#define MAX_VALUE MAX_ARRAY_SIZE
#define DEFAULT_SORT_DELAY 1
#define DEFAULT_SORT_ALGO "Bubble Sort"

// Shared variables
extern GtkWidget *drawing_area;
extern int array[MAX_ARRAY_SIZE];
extern int array_size;
extern int current_index;
extern int compare_index;
extern int sorted_boundary;
extern bool *sorted_flags;
extern gboolean sorting;
extern gboolean stop_requested;

extern GtkWidget *start_button, *stop_button, *new_array_button;
extern GtkWidget *algorithm_select;
extern GtkWidget *message_label, *overlay;
extern GtkWidget *theme_selector;
extern const char *current_algorithm;

extern int sort_delay;
extern int finished_message_hide_delay;

// Utility functions

void generate_random_array();
void reset_sort_state();

#endif
