#include "helper.h"

int array[MAX_ARRAY_SIZE];
int array_size = 0;
int current_index = -1;
int compare_index = -1;
int sorted_boundary = -1;
bool *sorted_flags = NULL;
gboolean sorting = FALSE;
gboolean stop_requested = FALSE;

int sort_delay = 50;
int finished_message_hide_delay = 3;

GtkWidget *drawing_area = NULL;
GtkWidget *start_button, *stop_button, *new_array_button;
GtkWidget *algorithm_select;
GtkWidget *message_label, *overlay;
GtkWidget *theme_selector;
const char *current_algorithm = "Bubble Sort";

int main(int argc, char *argv[]) {
    srand(time(NULL));

    #ifdef __APPLE__
    GtkosxApplication *osx_app = gtkosx_application_get();
    gtkosx_application_set_use_quartz_accelerators(osx_app, TRUE);
    #endif

    gtk_init(&argc, &argv);

    generate_random_array();

    // Fereastra principală
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sorting Visualizer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_present(GTK_WINDOW(window));

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
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Insert Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Merge Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Quick Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Heap Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Counting Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Radix Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Bucket Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_select), "Shell Sort");
    
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

    #ifdef __APPLE__
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
    #endif

    gtk_widget_set_sensitive(stop_button, FALSE);

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
    ThemeOption saved_theme = load_saved_theme();
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

    if (sorted_flags != NULL) {
        free(sorted_flags);
    }
    sorted_flags = NULL;

    return EXIT_SUCCESS;
}

void generate_random_array() {
    if (sorted_flags != NULL) {
        free(sorted_flags);
    }

    array_size = 50 + rand() % 51;

    for (int i = 0; i < array_size; i++) {
        array[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
    }

    sorted_flags = calloc(array_size, sizeof(bool));
    if (!sorted_flags) {
        fprintf(stderr, "Memory allocation failed for sorted_flags.\n");
        exit(EXIT_FAILURE);
    }
}


void reset_sort_state() {
    reset_sort_visual_state();
    sorting = FALSE;
    stop_requested = FALSE;
    gtk_widget_set_sensitive(start_button, TRUE);
    gtk_widget_queue_draw(drawing_area);
    gtk_widget_hide(message_label);
}