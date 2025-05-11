#include "helper.h"
#include "algoritms.h"

gboolean bubble_sort_step(gpointer data) {
    static int i = 0, j = 0;

    // If sorting is done or stopped
    if (stop_requested || i >= array_size - 1) {
        // Mark all elements as sorted
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;

        // Reset iteration state
        i = j = 0;
        current_index = compare_index = -1;

        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (j < array_size - i - 1) {
        current_index = j;
        compare_index = j + 1;

        if (array[j] > array[j + 1]) {
            int temp = array[j];
            array[j] = array[j + 1];
            array[j + 1] = temp;
        }

        j++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        // Mark the last element in this pass as sorted
        sorted_flags[array_size - i - 1] = true;

        j = 0;
        i++;
        return TRUE;
    }
}

gboolean selection_sort_step(gpointer data) {
    static int i = 0, j = 1, min_index = 0;

    if (stop_requested || i >= array_size - 1) {
        // Mark remaining elements as sorted
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;

        i = j = min_index = 0;
        current_index = compare_index = -1;

        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (j < array_size) {
        current_index = j;
        compare_index = min_index;

        if (array[j] < array[min_index]) {
            min_index = j;
        }

        j++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        // Swap the smallest element with position i
        if (min_index != i) {
            int temp = array[i];
            array[i] = array[min_index];
            array[min_index] = temp;
        }

        // Mark i as sorted
        sorted_flags[i] = true;

        i++;
        j = i + 1;
        min_index = i;

        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    }
}