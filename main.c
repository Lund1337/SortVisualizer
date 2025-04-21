#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_ARRAY_SIZE 100
#define MIN_VALUE -100
#define MAX_VALUE 100

static int array[MAX_ARRAY_SIZE];
static int array_size = 50;
static int current_index = -1;
static gboolean sorting = FALSE;
static gboolean stop_requested = FALSE;

static GtkWidget *drawing_area;
static GtkWidget *start_button, *stop_button, *reset_button, *new_array_button, *algorithm_select;
static const char *current_algorithm = "Bubble Sort";

void generate_random_array() {
    for (int i = 0; i < array_size; i++) {
        array[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
    }
    current_index = -1;
}

void draw_array(GtkWidget *widget, cairo_t *cr) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    int width = allocation.width;
    int height = allocation.height;
    int bar_width = width / array_size;

    for (int i = 0; i < array_size; i++) {
        double x = i * bar_width;
        double norm_val = (array[i] - MIN_VALUE) / (double)(MAX_VALUE - MIN_VALUE);
        double bar_height = norm_val * height * 0.9;

        // Color
        if (i == current_index)
            cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // red
        else
            cairo_set_source_rgb(cr, 0.2, 0.6, 1.0); // blue

        cairo_rectangle(cr, x, height - bar_height, bar_width - 1, bar_height);
        cairo_fill(cr);

        // Draw value above bar
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 10);
        char val[16];
        snprintf(val, sizeof(val), "%d", array[i]);
        cairo_move_to(cr, x + 2, height - bar_height - 4);
        cairo_show_text(cr, val);
    }
}

void reset_sort_state() {
    current_index = -1;
    sorting = FALSE;
    stop_requested = FALSE;
    gtk_widget_set_sensitive(start_button, TRUE);
    gtk_widget_queue_draw(drawing_area);
}

gboolean bubble_sort_step(gpointer data) {
    static int i = 0, j = 0;
    if (stop_requested || i >= array_size - 1) {
        i = j = 0;
        reset_sort_state();
        return FALSE;
    }
    if (j < array_size - i - 1) {
        if (array[j] > array[j + 1]) {
            int temp = array[j];
            array[j] = array[j + 1];
            array[j + 1] = temp;
        }
        current_index = j++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        j = 0;
        i++;
        return TRUE;
    }
}

gboolean selection_sort_step(gpointer data) {
    static int i = 0, j = 1, min_index = 0;
    if (stop_requested || i >= array_size - 1) {
        i = j = min_index = 0;
        reset_sort_state();
        return FALSE;
    }
    if (j < array_size) {
        if (array[j] < array[min_index]) {
            min_index = j;
        }
        current_index = j++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        int temp = array[i];
        array[i] = array[min_index];
        array[min_index] = temp;
        i++;
        j = i + 1;
        min_index = i;
        return TRUE;
    }
}

void on_start_clicked(GtkWidget *widget, gpointer user_data) {
    if (sorting) return;
    sorting = TRUE;
    gtk_widget_set_sensitive(start_button, FALSE);

    if (g_strcmp0(current_algorithm, "Bubble Sort") == 0) {
        g_timeout_add(50, bubble_sort_step, NULL);
    } else if (g_strcmp0(current_algorithm, "Selection Sort") == 0) {
        g_timeout_add(50, selection_sort_step, NULL);
    }
}

void on_stop_clicked(GtkWidget *widget, gpointer user_data) {
    stop_requested = TRUE;
}

void on_reset_clicked(GtkWidget *widget, gpointer user_data) {
    generate_random_array();
    reset_sort_state();
}

void on_new_array_clicked(GtkWidget *widget, gpointer user_data) {
    generate_random_array();
    gtk_widget_queue_draw(drawing_area);
}

void on_algorithm_changed(GtkComboBoxText *combo, gpointer user_data) {
    current_algorithm = gtk_combo_box_text_get_active_text(combo);
    reset_sort_state();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    gtk_init(&argc, &argv);

    generate_random_array();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sorting Visualizer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_vexpand(drawing_area, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_array), NULL);

    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), controls, FALSE, FALSE, 5);

    start_button = gtk_button_new_with_label("Start");
    stop_button = gtk_button_new_with_label("Stop");
    reset_button = gtk_button_new_with_label("Reset Array");
    new_array_button = gtk_button_new_with_label("New Random Array");
    algorithm_select = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Bubble Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Selection Sort");
    gtk_combo_box_set_active(GTK_COMBO_BOX(algorithm_select), 0);

    gtk_box_pack_start(GTK_BOX(controls), start_button, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(controls), stop_button, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(controls), reset_button, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(controls), new_array_button, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(controls), algorithm_select, TRUE, TRUE, 2);

    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_clicked), NULL);
    g_signal_connect(stop_button, "clicked", G_CALLBACK(on_stop_clicked), NULL);
    g_signal_connect(reset_button, "clicked", G_CALLBACK(on_reset_clicked), NULL);
    g_signal_connect(new_array_button, "clicked", G_CALLBACK(on_new_array_clicked), NULL);
    g_signal_connect(algorithm_select, "changed", G_CALLBACK(on_algorithm_changed), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}