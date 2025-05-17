#include "helper.h"
#include "gui.h"
typedef struct {
    double r, g, b;
} RGB;

const RGB COLOR_GRAY    = {0.9, 0.9, 0.9};  // Gray
const RGB COLOR_CURRENT = {1.0, 0.0, 0.0};  // Red
const RGB COLOR_COMPARE = {0.5, 0.0, 0.5};  // Purple
const RGB COLOR_SORTED  = {0.0, 0.8, 0.0};  // Green
const RGB COLOR_DEFAULT = {0.2, 0.6, 1.0};  // Blue

const char* theme_option_to_string(ThemeOption theme) {
    switch (theme) {
        case THEME_LIGHT: return "Light";
        case THEME_DARK: return "Dark";
        case THEME_SYSTEM: return "System";
        default: return "Unknown";
    }
}

ThemeOption string_to_theme_option(const char* str) {
    if (strcmp(str, "Light") == 0) return THEME_LIGHT;
    if (strcmp(str, "Dark") == 0) return THEME_DARK;
    if (strcmp(str, "System") == 0) return THEME_SYSTEM;
    return THEME_SYSTEM; // default fallback
}

void draw_array(GtkWidget *widget, cairo_t *cr) {
    if (array_size == 0) return;

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    int width = allocation.width;
    int height = allocation.height;
    int bar_width = MAX(1, width / array_size);

    // Linie la 0
    double zero_level = (0 - MIN_VALUE) / (double)(MAX_VALUE - MIN_VALUE);
    double zero_y = height - zero_level * height * 0.9;

    cairo_set_source_rgb(cr, COLOR_GRAY.r, COLOR_GRAY.g, COLOR_GRAY.b); // dark grey
    cairo_set_line_width(cr, 1.0);
    cairo_move_to(cr, 0, zero_y);
    cairo_line_to(cr, width, zero_y);
    cairo_stroke(cr);

    for (int i = 0; i < array_size; i++) {
        double x = i * bar_width;
        double norm_val = (array[i] - MIN_VALUE) / (double)(MAX_VALUE - MIN_VALUE);
        double bar_height = norm_val * height * 0.9;

        if (i == current_index) {
            cairo_set_source_rgb(cr, COLOR_CURRENT.r, COLOR_CURRENT.g, COLOR_CURRENT.b); // Red
        } else if (i == compare_index) {
            cairo_set_source_rgb(cr, COLOR_COMPARE.r, COLOR_COMPARE.g, COLOR_COMPARE.b); // Purple
        } else if (sorted_flags && sorted_flags[i]) {
            cairo_set_source_rgb(cr, COLOR_SORTED.r, COLOR_SORTED.g, COLOR_SORTED.b); // Green
        } else {
            cairo_set_source_rgb(cr, COLOR_DEFAULT.r, COLOR_DEFAULT.g, COLOR_DEFAULT.b); // Blue
        }

        cairo_rectangle(cr, x, height - bar_height, bar_width - 0.5, bar_height);
        cairo_fill(cr);

        // Draw value above bar
        if (bar_width >= 3) {
            cairo_set_source_rgb(cr, 0.5, 0.5, 0.5); // gray
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, 8);
            char val[16];
            snprintf(val, sizeof(val), "%d", array[i]);
            cairo_move_to(cr, x + 2, height - bar_height - 4);
            cairo_show_text(cr, val);
        }
    }
}

void reset_sort_visual_state() {
    current_index = -1;
    compare_index = -1;
    sorted_boundary = -1;
    if (sorted_flags != NULL) {
        memset(sorted_flags, 0, array_size * sizeof(bool));
    }
}

void show_finished_message() {
    gtk_widget_show(message_label);
    g_timeout_add(finished_message_hide_delay * 1000, (GSourceFunc)gtk_widget_hide, message_label);
    gtk_widget_set_sensitive(new_array_button, TRUE);
}

void on_start_clicked(GtkWidget *widget, gpointer user_data) {
    if (sorting) return;
    sorting = TRUE;
    gtk_widget_set_sensitive(stop_button, TRUE);
    gtk_widget_set_sensitive(start_button, FALSE);
    gtk_widget_set_sensitive(new_array_button, FALSE);
    gtk_widget_hide(message_label);

    reset_sort();
    start_sorting(current_algorithm);
}

sort_step_func get_sort_step_function(const gchar *algorithm_name) {
    if (g_strcmp0(algorithm_name, "Bubble Sort") == 0) {
        return bubble_sort_step;
    } else if (g_strcmp0(algorithm_name, "Selection Sort") == 0) {
        return selection_sort_step;
    } else if (g_strcmp0(algorithm_name, "Insert Sort") == 0) {
        return insert_sort_step;
    } else if (g_strcmp0(algorithm_name, "Merge Sort") == 0) {
        return merge_sort_step;
    } else if (g_strcmp0(algorithm_name, "Quick Sort") == 0) {
        return quick_sort_step;
    } else if (g_strcmp0(algorithm_name, "Heap Sort") == 0) {
        return heap_sort_step;
    } else if (g_strcmp0(algorithm_name, "Counting Sort") == 0) {
        return counting_sort_step;
    } else if (g_strcmp0(algorithm_name, "Radix Sort") == 0) {
        return radix_sort_step;
    } else if (g_strcmp0(algorithm_name, "Bucket Sort") == 0) {
        return bucket_sort_step;
    } else if (g_strcmp0(algorithm_name, "Shell Sort") == 0) {
        return shell_sort_step;
    } else if (g_strcmp0(algorithm_name, "Tim Sort") == 0) {
        return tim_sort_step;
    } else if (g_strcmp0(algorithm_name, "Intro Sort") == 0) {
        return intro_sort_step;
    } else {
        g_warning("Unknown sorting algorithm: %s", algorithm_name);
        return NULL;
    }
}

void start_sorting(const gchar *current_algorithm) {
    sort_step_func sort_step = get_sort_step_function(current_algorithm);

    if (sort_step != NULL) {
        g_timeout_add(sort_delay, sort_step, NULL);
    }
}

void on_stop_clicked(GtkWidget *widget, gpointer user_data) {
    stop_requested = TRUE;
    gtk_widget_set_sensitive(new_array_button, TRUE);
}

void on_new_array_clicked(GtkWidget *widget, gpointer user_data) {
    generate_random_array();
    reset_sort_state();
    gtk_widget_set_sensitive(stop_button, FALSE);
    gtk_widget_queue_draw(drawing_area);
}

void on_algorithm_changed(GtkComboBoxText *combo, gpointer user_data) {
    current_algorithm = gtk_combo_box_text_get_active_text(combo);
    reset_sort_state();
}

void on_speed_changed(GtkRange *range, gpointer user_data) {
    sort_delay = (int)gtk_range_get_value(range);
}

void save_theme_preference(ThemeOption theme) {
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

    ThemeOption theme = string_to_theme_option(selected);
    GtkSettings *settings = gtk_settings_get_default();

    if (theme == THEME_DARK) {
        g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
    } else {
        g_object_set(settings, "gtk-application-prefer-dark-theme", FALSE, NULL);
    }

    save_theme_preference(theme);
}

ThemeOption load_saved_theme() {
    GKeyFile *key_file = g_key_file_new();
    gchar *config_path = g_build_filename(g_get_user_config_dir(), "SortVisualizer", "settings.ini", NULL);
    ThemeOption theme = THEME_SYSTEM;

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

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if ((event->state & GDK_META_MASK) && event->keyval == GDK_KEY_q) {
        gtk_window_close(GTK_WINDOW(widget));
        return TRUE;
    }
    return FALSE;
}