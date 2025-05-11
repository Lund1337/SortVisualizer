#include "helper.h"
#include "algoritms.h"
typedef struct {
    int left;
    int right;
} QuickFrame;

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
    static int key;
    static gboolean inserting = FALSE;

    if (stop_requested || i >= array_size) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;

        i = j = 0;
        inserting = FALSE;
        current_index = compare_index = -1;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (!inserting) {
        key = array[i];
        j = i - 1;
        inserting = TRUE;
    }

    current_index = j;
    compare_index = j + 1;

    if (j >= 0 && array[j] > key) {
        array[j + 1] = array[j];
        j--;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        array[j + 1] = key;
        i++;
        inserting = FALSE;
        gtk_widget_queue_draw(drawing_area);
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

static QuickFrame quick_stack[MAX_STACK_SIZE];
static int quick_stack_top = -1;

static int quick_left = 0, quick_right = 0;
static int quick_current_left = 0;
static int quick_pivot = -1;
static int quick_i = -1;
static gboolean quick_partitioning = FALSE;

void push_quick(int left, int right) {
    if (quick_stack_top < MAX_STACK_SIZE - 1) {
        quick_stack[++quick_stack_top].left = left;
        quick_stack[quick_stack_top].right = right;
    }
}

gboolean pop_quick(int *left, int *right) {
    if (quick_stack_top >= 0) {
        *left = quick_stack[quick_stack_top].left;
        *right = quick_stack[quick_stack_top].right;
        quick_stack_top--;
        return TRUE;
    }
    return FALSE;
}

gboolean quick_sort_step(gpointer data) {
    if (stop_requested || (quick_stack_top == -1 && !quick_partitioning)) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = TRUE;

        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (!quick_partitioning) {
        if (!pop_quick(&quick_left, &quick_right))
            return TRUE;

        if (quick_left >= quick_right)
            return TRUE;

        quick_current_left = quick_left;
        quick_pivot = quick_right;
        quick_i = quick_left;
        quick_partitioning = TRUE;
    }

    if (quick_i < quick_pivot) {
        current_index = quick_i;
        compare_index = quick_pivot;

        if (array[quick_i] < array[quick_pivot]) {
            int temp = array[quick_i];
            array[quick_i] = array[quick_left];
            array[quick_left] = temp;
            quick_left++;
        }

        quick_i++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        // Final pivot swap
        int temp = array[quick_pivot];
        array[quick_pivot] = array[quick_left];
        array[quick_left] = temp;

        sorted_flags[quick_left] = TRUE;

        // Push new partitions
        push_quick(quick_left + 1, quick_right);               // Right partition
        push_quick(quick_current_left, quick_left - 1);        // Left partition

        quick_partitioning = FALSE;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    }
}

void reset_quick_sort() {
    quick_stack_top = -1;
    quick_partitioning = FALSE;
    quick_left = quick_right = quick_pivot = quick_i = -1;
    current_index = compare_index = -1;

    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = FALSE;

    push_quick(0, array_size - 1);
    gtk_widget_queue_draw(drawing_area);
}

static gboolean heap_phase = TRUE;  // TRUE = building heap, FALSE = sorting
static int heap_build_index = 0;

static int heap_size = 0;
static int heap_i = -1;

gboolean heap_sort_step(gpointer data) {
    if (stop_requested) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;
        
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    // Phase 1: Build the heap from array (bottom-up)
    if (heap_phase) {
        if (heap_build_index < 0) {
            heap_phase = FALSE;
            heap_size = array_size;
            heap_i = 0;
            return TRUE;
        }

        heap_i = heap_build_index;
        heapify_single_step(array_size);
        heap_build_index--;

        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    }

    // Phase 2: Extract max and re-heapify
    if (heap_i < heap_size) {
        gboolean still_heapifying = heapify_single_step(heap_size);
        gtk_widget_queue_draw(drawing_area);
        if (still_heapifying)
            return TRUE;
    }

    if (heap_size <= 1) {
        sorted_flags[0] = true;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    // Swap max (root) with last unsorted element
    int temp = array[0];
    array[0] = array[heap_size - 1];
    array[heap_size - 1] = temp;

    sorted_flags[heap_size - 1] = true;
    heap_size--;

    heap_i = 0; // restart sift-down from root
    gtk_widget_queue_draw(drawing_area);
    return TRUE;
}

gboolean heapify_single_step(int n) {
    int left = 2 * heap_i + 1;
    int right = 2 * heap_i + 2;
    int largest = heap_i;

    if (left < n && array[left] > array[largest])
        largest = left;
    if (right < n && array[right] > array[largest])
        largest = right;

    current_index = heap_i;
    compare_index = largest;

    if (largest != heap_i) {
        // Swap and continue heapifying in next frame
        int temp = array[heap_i];
        array[heap_i] = array[largest];
        array[largest] = temp;

        heap_i = largest;
        return TRUE;  // Continue in next step
    }

    heap_i = n; // Done with this heapify pass
    return FALSE;
}

void reset_heap_sort() {
    heap_size = 0;
    heap_phase = TRUE;
    heap_build_index = array_size / 2 - 1;
    heap_i = -1;
    current_index = compare_index = -1;

    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;

    gtk_widget_queue_draw(drawing_area);
}

static int count[VALUE_RANGE];
static int current_value = MIN_VALUE;
static int rebuild_index = 0;
static gboolean counting_phase = TRUE;

gboolean counting_sort_step(gpointer data) {
    if (stop_requested) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    // Phase 1: Count frequencies
    if (counting_phase) {
        memset(count, 0, sizeof(count));
        for (int i = 0; i < array_size; ++i) {
            int index = array[i] + OFFSET;
            if (index >= 0 && index < VALUE_RANGE)
                count[index]++;
        }
        counting_phase = FALSE;
        current_value = MIN_VALUE;
        rebuild_index = 0;
        return TRUE;
    }

    // Phase 2: Reconstruct sorted array step-by-step
    while (current_value <= MAX_VALUE && count[current_value + OFFSET] == 0)
        current_value++;

    if (current_value > MAX_VALUE || rebuild_index >= array_size) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    array[rebuild_index] = current_value;
    sorted_flags[rebuild_index] = true;
    count[current_value + OFFSET]--;
    current_index = rebuild_index;
    compare_index = current_value + OFFSET;
    rebuild_index++;

    gtk_widget_queue_draw(drawing_area);
    return TRUE;
}

void reset_counting_sort() {
    memset(count, 0, sizeof(count));
    current_value = MIN_VALUE;
    rebuild_index = 0;
    counting_phase = TRUE;
    current_index = compare_index = -1;
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;
    gtk_widget_queue_draw(drawing_area);
}

static int exp = 1;

gboolean radix_sort_step(gpointer data) {
    if (stop_requested || exp > 1000) {  // Arbitrary limit for digits
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    int output[array_size], count[10] = {0};
    int output_neg[array_size]; // To store sorted negative values
    int count_neg[10] = {0};  // To count digits for negative numbers

    // Step 1: Separate positive and negative values
    int pos_index = 0;
    int neg_index = 0;

    for (int i = 0; i < array_size; i++) {
        if (array[i] < 0) {
            neg_index++;
        } else {
            pos_index++;
        }
    }

    // Step 2: Count occurrences of digits based on current 'exp'
    for (int i = 0; i < array_size; i++) {
        if (array[i] >= 0) {
            // For positive values, use the absolute value
            int digit = (array[i] / exp) % 10;
            count[digit]++;
        } else {
            // For negative values, use the absolute value and store them separately
            int digit = (abs(array[i]) / exp) % 10;
            count_neg[digit]++;
        }
    }

    // Step 3: Update count to hold the actual positions
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
        count_neg[i] += count_neg[i - 1];
    }

    // Step 4: Sort negative values (ascending)
    for (int i = 0; i < array_size; i++) {
    if (array[i] < 0) {
        int digit = ((-array[i]) / exp) % 10;
        output_neg[count_neg[digit] - 1] = array[i];
        count_neg[digit]--;
        }
    }
    
    // Step 5: Sort positive values (ascending)
    for (int i = array_size - 1; i >= 0; i--) {
        if (array[i] >= 0) {
            int digit = (array[i] / exp) % 10;
            output[count[digit] - 1] = array[i];
            count[digit]--;
        }
    }

    // Step 6: Merge sorted negative and positive arrays
    int radix_reset_index = 0;
    for (int i = neg_index; i > 0; i--) {
        array[radix_reset_index++] = output_neg[i];  // Place sorted negatives first
    }
    for (int i = 0; i < pos_index; i++) {
        array[radix_reset_index++] = output[i];      // Place sorted positives after
    }

    // Step 7: Move to the next digit (exp * 10)
    exp *= 10;

    // Trigger redraw
    gtk_widget_queue_draw(drawing_area);
    return TRUE;
}

void reset_radix_sort() {
    exp = 1;
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;
    gtk_widget_queue_draw(drawing_area);
}

static int bucket_index = 0;
static GArray* buckets[BUCKETS];
static int merge_pos = 0;
static int array_write_index = 0;

void reset_bucket_sort() {
    for (int i = 0; i < BUCKETS; i++) {
        if (buckets[i]) {
            g_array_free(buckets[i], TRUE);
            buckets[i] = NULL;
        }
    }

    bucket_index = 0;
    merge_pos = 0;
    array_write_index = 0;

    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;

    gtk_widget_queue_draw(drawing_area);
}

int int_compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void initialize_buckets() {
    for (int i = 0; i < BUCKETS; i++)
        buckets[i] = g_array_new(FALSE, FALSE, sizeof(int));

    int range = MAX_VALUE - MIN_VALUE + 1;

    for (int i = 0; i < array_size; i++) {
        int norm_val = array[i] - MIN_VALUE;
        int index = (norm_val * BUCKETS) / range;
        if (index >= BUCKETS) index = BUCKETS - 1;
        g_array_append_val(buckets[index], array[i]);
    }

    for (int i = 0; i < BUCKETS; i++) {
        if (buckets[i]->len > 1)
            qsort(buckets[i]->data, buckets[i]->len, sizeof(int), int_compare);
    }
}

gboolean bucket_sort_step(gpointer data) {
    if (bucket_index == 0 && merge_pos == 0 && array_write_index == 0) {
        initialize_buckets();
    }

    if (stop_requested || bucket_index >= BUCKETS) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;

        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (merge_pos < buckets[bucket_index]->len) {
        int val = g_array_index(buckets[bucket_index], int, merge_pos);
        array[array_write_index] = val;
        sorted_flags[array_write_index] = true;
        merge_pos++;
        array_write_index++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    }

    // Move to next bucket
    bucket_index++;
    merge_pos = 0;
    return TRUE;
}

static int gap = 0, i = 0, j = 0;
static int temp;
static gboolean inserting = FALSE;

gboolean shell_sort_step(gpointer data) {
    if (gap == 0) {
        gap = array_size / 2;
        if (gap == 0) {
            for (int k = 0; k < array_size; ++k)
                sorted_flags[k] = true;
            gtk_widget_queue_draw(drawing_area);
            show_finished_message();
            return FALSE;
        }
        i = gap;
    }

    if (stop_requested) {
        reset_shell_sort();
        gtk_widget_queue_draw(drawing_area);
        return FALSE;
    }

    if (i < array_size) {
        if (!inserting) {
            temp = array[i];
            j = i;
            inserting = TRUE;
        }

        current_index = j;
        compare_index = (j >= gap) ? j - gap : -1;

        if (j >= gap && array[j - gap] > temp) {
            array[j] = array[j - gap];
            j -= gap;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        } else {
            array[j] = temp;
            i++;
            inserting = FALSE;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }
    } else {
        gap /= 2;
        if (gap == 0) {
            for (int k = 0; k < array_size; ++k)
                sorted_flags[k] = true;
            gtk_widget_queue_draw(drawing_area);
            show_finished_message();
            return FALSE;
        }
        i = gap;
        inserting = FALSE;
        return TRUE;
    }
}

void reset_shell_sort() {
    gap = 0;
    i = j = 0;
    inserting = FALSE;
    current_index = compare_index = -1;
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;
    gtk_widget_queue_draw(drawing_area);
}

void reset_sort() {
    reset_merge_sort();
    reset_quick_sort();
    reset_heap_sort();
    reset_counting_sort();
    reset_radix_sort();
    reset_bucket_sort();
    reset_shell_sort();
}