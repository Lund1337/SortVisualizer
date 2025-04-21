#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_ARRAY_SIZE 100
#define MIN_VALUE -100
#define MAX_VALUE 100

static int array[MAX_ARRAY_SIZE];
static int array_size;
static int current_index = -1;
static gboolean sorting = FALSE;
static gboolean stop_requested = FALSE;

static int sort_delay = 50;
static int finished_message_hide_delay = 3;

static GtkWidget *drawing_area;
static GtkWidget *start_button, *stop_button, *new_array_button;
static GtkWidget *algorithm_select;
static GtkWidget *message_label, *overlay;
static GtkWidget *theme_selector;
static const char *current_algorithm = "Bubble Sort";

enum ThemeOption {
    THEME_SYSTEM,
    THEME_DARK,
    THEME_LIGHT
};

const char* theme_option_to_string(enum ThemeOption theme) {
    switch (theme) {
        case THEME_DARK: return "Dark";
        case THEME_LIGHT: return "Light";
        default: return "System Theme";
    }
}

enum ThemeOption string_to_theme_option(const char* str) {
    if (g_strcmp0(str, "Dark") == 0)
        return THEME_DARK;
    else if (g_strcmp0(str, "Light") == 0)
        return THEME_LIGHT;
    else
        return THEME_SYSTEM;
}

void generate_random_array() {
    array_size = 50 + rand() % 51;
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

    // Linie la 0
    double zero_level = (0 - MIN_VALUE) / (double)(MAX_VALUE - MIN_VALUE);
    double zero_y = height - zero_level * height * 0.9;

    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9); // dark grey
    cairo_set_line_width(cr, 1.0);
    cairo_move_to(cr, 0, zero_y);
    cairo_line_to(cr, width, zero_y);
    cairo_stroke(cr);

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
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5); // gray
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
    gtk_widget_hide(message_label);
}

void show_finished_message() {
    gtk_widget_show(message_label);
    g_timeout_add(finished_message_hide_delay * 1000, (GSourceFunc)gtk_widget_hide, message_label);
}

gboolean bubble_sort_step(gpointer data) {
    static int i = 0, j = 0;
    if (stop_requested || i >= array_size - 1) {
        i = j = 0;
        reset_sort_state();
        show_finished_message();
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
        show_finished_message();
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
    gtk_widget_hide(message_label);

    if (g_strcmp0(current_algorithm, "Bubble Sort") == 0) {
        g_timeout_add(sort_delay, bubble_sort_step, NULL);
    } else if (g_strcmp0(current_algorithm, "Selection Sort") == 0) {
        g_timeout_add(sort_delay, selection_sort_step, NULL);
    }
}

void on_stop_clicked(GtkWidget *widget, gpointer user_data) {
    stop_requested = TRUE;
}

void on_new_array_clicked(GtkWidget *widget, gpointer user_data) {
    generate_random_array();
    reset_sort_state();
    gtk_widget_queue_draw(drawing_area);
}

void on_algorithm_changed(GtkComboBoxText *combo, gpointer user_data) {
    current_algorithm = gtk_combo_box_text_get_active_text(combo);
    reset_sort_state();
}

void on_speed_changed(GtkRange *range, gpointer user_data) {
    sort_delay = (int)gtk_range_get_value(range);
}

void save_theme_preference(enum ThemeOption theme) {
    GKeyFile *key_file = g_key_file_new();
    gchar *config_path = g_build_filename(g_get_user_config_dir(), "SortVisualizer", "settings.ini", NULL);

    g_key_file_set_string(key_file, "Preferences", "Theme", theme_option_to_string(theme));

    g_mkdir_with_parents(g_path_get_dirname(config_path), 0700);
    gsize length;
    gchar *data = g_key_file_to_data(key_file, &length, NULL);
    g_file_set_contents(config_path, data, length, NULL);

    g_free(data);
    g_key_file_unref(key_file);
    g_free(config_path);
}

void on_theme_changed(GtkComboBoxText *combo, gpointer user_data) {
    const gchar *selected = gtk_combo_box_text_get_active_text(combo);
    if (!selected) return;

    enum ThemeOption theme = string_to_theme_option(selected);
    GtkSettings *settings = gtk_settings_get_default();

    if (theme == THEME_DARK) {
        g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
    } else {
        g_object_set(settings, "gtk-application-prefer-dark-theme", FALSE, NULL);
    }

    save_theme_preference(theme);
}

enum ThemeOption load_saved_theme() {
    GKeyFile *key_file = g_key_file_new();
    gchar *config_path = g_build_filename(g_get_user_config_dir(), "SortVisualizer", "settings.ini", NULL);
    enum ThemeOption theme = THEME_SYSTEM;

    if (g_key_file_load_from_file(key_file, config_path, G_KEY_FILE_NONE, NULL)) {
        gchar *theme_str = g_key_file_get_string(key_file, "Preferences", "Theme", NULL);
        if (theme_str) {
            theme = string_to_theme_option(theme_str);
            g_free(theme_str);
        }
    }

    g_key_file_unref(key_file);
    g_free(config_path);
    return theme;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    gtk_init(&argc, &argv);

    generate_random_array();

    // Fereastra principală
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sorting Visualizer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Container vertical principal
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // ⚠️ MUTAT SUS — creați înainte de a fi adăugat în overlay
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_vexpand(drawing_area, TRUE);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_array), NULL);

    // Overlay pentru a suprapune mesajul peste zona de desenare
    overlay = gtk_overlay_new();
    gtk_box_pack_start(GTK_BOX(vbox), overlay, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(overlay), drawing_area);

    // Mesaj de finalizare
    message_label = gtk_label_new("Finished Sorting");
    gtk_widget_set_name(message_label, "finished_label");
    gtk_widget_set_halign(message_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(message_label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(message_label, 15);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), message_label);
    gtk_widget_hide(message_label);

    // Container pentru controale
    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), controls, FALSE, FALSE, 5);

    // Butoane
    start_button = gtk_button_new_with_label("Start");
    stop_button = gtk_button_new_with_label("Stop");
    new_array_button = gtk_button_new_with_label("New Array");
    theme_selector = gtk_combo_box_text_new();

    gtk_box_pack_start(GTK_BOX(controls), start_button, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(controls), stop_button, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(controls), new_array_button, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(controls), theme_selector, FALSE, FALSE, 10);

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(theme_selector), "System Theme");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(theme_selector), "Dark");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(theme_selector), "Light");
    gtk_combo_box_set_active(GTK_COMBO_BOX(theme_selector), 0);

    // ComboBox pentru alegerea algoritmului
    algorithm_select = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Bubble Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Selection Sort");
    gtk_combo_box_set_active(GTK_COMBO_BOX(algorithm_select), 0);
    gtk_box_pack_start(GTK_BOX(controls), algorithm_select, TRUE, TRUE, 2);

    // Slider pentru viteza de sortare
    GtkWidget *speed_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 300, 5);
    gtk_range_set_value(GTK_RANGE(speed_scale), sort_delay);
    gtk_scale_set_draw_value(GTK_SCALE(speed_scale), TRUE);
    gtk_scale_set_value_pos(GTK_SCALE(speed_scale), GTK_POS_TOP);
    gtk_widget_set_tooltip_text(speed_scale, "Adjust sorting speed (ms)");
    gtk_box_pack_start(GTK_BOX(vbox), speed_scale, FALSE, FALSE, 5);

    // Conectarea semnalelor
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_clicked), NULL);
    g_signal_connect(stop_button, "clicked", G_CALLBACK(on_stop_clicked), NULL);
    g_signal_connect(new_array_button, "clicked", G_CALLBACK(on_new_array_clicked), NULL);
    g_signal_connect(algorithm_select, "changed", G_CALLBACK(on_algorithm_changed), NULL);
    g_signal_connect(speed_scale, "value-changed", G_CALLBACK(on_speed_changed), NULL);
    g_signal_connect(theme_selector, "changed", G_CALLBACK(on_theme_changed), NULL);

    // CSS pentru mesaj
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#finished_label { background-color: rgba(0,0,0,0.6); color: white; padding: 6px 12px; border-radius: 6px; font-size: 12px; }",
        -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    gtk_widget_show_all(window);
    gtk_widget_hide(message_label);

    // Theme should be set after widgets are shown
    enum ThemeOption saved_theme = load_saved_theme();
    GtkSettings *settings = gtk_settings_get_default();
    if (G_IS_OBJECT(settings)) {
        if (saved_theme == THEME_DARK) {
            g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
        } else {
            g_object_set(settings, "gtk-application-prefer-dark-theme", FALSE, NULL);
        }
    }

    switch (saved_theme) {
        case THEME_DARK: gtk_combo_box_set_active(GTK_COMBO_BOX(theme_selector), 1); break;
        case THEME_LIGHT: gtk_combo_box_set_active(GTK_COMBO_BOX(theme_selector), 2); break;
        default: gtk_combo_box_set_active(GTK_COMBO_BOX(theme_selector), 0); break;
    }
    
    gtk_main();

    return EXIT_SUCCESS;
}
