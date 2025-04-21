// SortVisualizer - GTK3 GUI modulară pentru algoritmi de sortare în C
// Compilează cu: gcc -o sortviz main.c `pkg-config --cflags --libs gtk+-3.0`

#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100

static int array[ARRAY_SIZE];
static int current_index = 0;
static gboolean sorting = FALSE;
static GtkWidget *drawing_area;

void generate_random_array() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100;
    }
}

gboolean bubble_sort_step(GtkWidget *widget) {
    static int i = 0, j = 0;
    if (i < ARRAY_SIZE - 1) {
        if (j < ARRAY_SIZE - i - 1) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
            current_index = j;
            j++;
            gtk_widget_queue_draw(widget);
            return TRUE;
        } else {
            j = 0;
            i++;
            return TRUE;
        }
    }
    sorting = FALSE;
    return FALSE;
}

void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    int width = allocation.width;
    int height = allocation.height;
    int bar_width = width / ARRAY_SIZE;

    for (int i = 0; i < ARRAY_SIZE; i++) {
        double x = i * bar_width;
        double bar_height = (array[i] / 100.0) * height;

        if (i == current_index)
            cairo_set_source_rgb(cr, 1, 0, 0);  // red
        else
            cairo_set_source_rgb(cr, 0.2, 0.8, 0.2);  // green

        cairo_rectangle(cr, x, height - bar_height, bar_width - 1, bar_height);
        cairo_fill(cr);
    }
}

void on_start_sort(GtkButton *button, gpointer user_data) {
    if (!sorting) {
        sorting = TRUE;
        g_timeout_add(30, (GSourceFunc)bubble_sort_step, drawing_area);
    }
}

void on_reset(GtkButton *button, gpointer user_data) {
    generate_random_array();
    current_index = 0;
    sorting = FALSE;
    gtk_widget_queue_draw(drawing_area);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    generate_random_array();

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SortVisualizer GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *start_btn = gtk_button_new_with_label("Start Sort");
    GtkWidget *reset_btn = gtk_button_new_with_label("Reset Array");
    gtk_box_pack_start(GTK_BOX(button_box), start_btn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(button_box), reset_btn, FALSE, FALSE, 5);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_vexpand(drawing_area, TRUE);
    gtk_widget_set_hexpand(drawing_area, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 5);

    g_signal_connect(start_btn, "clicked", G_CALLBACK(on_start_sort), NULL);
    g_signal_connect(reset_btn, "clicked", G_CALLBACK(on_reset), NULL);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}