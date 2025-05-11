#ifndef GUI_H
#define GUI_H

#include "helper.h"

typedef enum {
    THEME_LIGHT,
    THEME_DARK,
    THEME_SYSTEM
} ThemeOption;

const char* theme_option_to_string(ThemeOption theme);
ThemeOption string_to_theme_option(const char* str);
void draw_array(GtkWidget *widget, cairo_t *cr);
void reset_sort_visual_state();
void show_finished_message();
void on_start_clicked(GtkWidget *widget, gpointer user_data);
void on_stop_clicked(GtkWidget *widget, gpointer user_data);
void on_new_array_clicked(GtkWidget *widget, gpointer user_data);
void on_algorithm_changed(GtkComboBoxText *combo, gpointer user_data);
void on_speed_changed(GtkRange *range, gpointer user_data);
void save_theme_preference(ThemeOption theme);
void on_theme_changed(GtkComboBoxText *combo, gpointer user_data);
ThemeOption load_saved_theme();
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
typedef gboolean (*sort_step_func)(gpointer data);
sort_step_func get_sort_step_function(const gchar *algorithm_name);
void start_sorting(const gchar *current_algorithm);

#endif
