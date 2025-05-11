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

gboolean insert_sort_step(gpointer data) {
    static int i = 1, j = 0;

    // If sorting is done or stopped
    if (stop_requested || i >= array_size) {
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

    // Insert the element at index i into the sorted portion
    if (j < i) {
        current_index = j;
        compare_index = j + 1;

        // If the current element is smaller than the element before it, swap them
        if (array[j] > array[j + 1]) {
            int temp = array[j];
            array[j] = array[j + 1];
            array[j + 1] = temp;
        }

        j++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        // If we've compared all elements in this pass, move to the next element
        sorted_flags[i - 1] = true;  // Mark element i-1 as sorted (it's now in the correct position)
        j = 0;
        i++;  // Move to the next element to insert into the sorted portion
        return TRUE;
    }
}

static int merge_width = 1;
static int merge_left = 0, merge_right = 0, merge_mid = 0;

gboolean merge_sort_step(gpointer data) {
    if (stop_requested || merge_width >= array_size) {
        // Mark all elements as sorted
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;

        // Reset iteration state
        merge_width = merge_left = merge_right = merge_mid = 0;
        current_index = compare_index = -1;

        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (merge_left < array_size) {
        merge_mid = merge_left + merge_width;
        merge_right = merge_left + 2 * merge_width;
        
        if (merge_mid > array_size) merge_mid = array_size;
        if (merge_right > array_size) merge_right = array_size;

        // Merge two sorted subarrays: array[merge_left:merge_mid] and array[merge_mid:merge_right]
        int temp[merge_right - merge_left];
        int i = merge_left, j = merge_mid, k = 0;
        
        while (i < merge_mid && j < merge_right) {
            current_index = i;
            compare_index = j;

            if (array[i] <= array[j]) {
                temp[k++] = array[i++];
            } else {
                temp[k++] = array[j++];
            }
        }

        while (i < merge_mid) temp[k++] = array[i++];
        while (j < merge_right) temp[k++] = array[j++];

        // Copy merged elements back into original array
        for (int m = 0; m < k; ++m)
            array[merge_left + m] = temp[m];

        merge_left += 2 * merge_width;

        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        merge_left = 0;
        merge_width *= 2;  // Increase segment merge_width for next merge phase
        return TRUE;
    }
}

void reset_merge_sort() {
    // Reset sorting parameters
    merge_width = 1;
    merge_left = merge_right = merge_mid = 0;
    current_index = compare_index = -1;

    // Clear sorted flags
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;

    // Redraw the visualization
    gtk_widget_queue_draw(drawing_area);
}

static int left = 0, right = MAX_ARRAY_SIZE - 1;
static int pivot_index = -1;
static int partition_index = -1;
static gboolean partitioning = FALSE;

gboolean quick_sort_step(gpointer data) {
    right = array_size - 1;
    if (stop_requested || left >= right) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;

        reset_quick_sort();  
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (!partitioning) {
        pivot_index = right;  // Choose last element as pivot
        partition_index = left;
        partitioning = TRUE;
    }

    if (partition_index < pivot_index) {
        current_index = partition_index;
        compare_index = pivot_index;

        if (array[partition_index] < array[pivot_index]) {
            int temp = array[partition_index];
            array[partition_index] = array[left];
            array[left] = temp;
            left++; 
        }
        partition_index++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        // Place pivot in correct position
        int temp = array[pivot_index];
        array[pivot_index] = array[left];
        array[left] = temp;
        sorted_flags[left] = true;

        // Update boundaries for next partition
        right = left - 1;
        left++;
        partitioning = FALSE;
        return TRUE;
    }

    return FALSE;
}

void reset_quick_sort() {
    left = 0;
    right = array_size - 1;
    pivot_index = -1;
    partition_index = -1;
    partitioning = FALSE;
    current_index = compare_index = -1;

    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;

    gtk_widget_queue_draw(drawing_area);
}

gboolean heap_sort_step(gpointer data) {
    return TRUE;

}

gboolean couting_sort_step(gpointer data) {
    return TRUE;

}

gboolean radix_sort_step(gpointer data) {
    return TRUE;

}

gboolean bucket_sort_step(gpointer data) {
    return TRUE;

}

gboolean shell_sort_step(gpointer data) {
    return TRUE;

}

void reset_sort() {
    reset_merge_sort();
    reset_quick_sort();
}