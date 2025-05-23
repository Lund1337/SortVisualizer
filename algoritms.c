#include "helper.h"
#include "algoritms.h"

/*
______       _     _     _        _____            _   
| ___ \     | |   | |   | |      /  ___|          | |  
| |_/ /_   _| |__ | |__ | | ___  \ `--.  ___  _ __| |_ 
| ___ \ | | | '_ \| '_ \| |/ _ \  `--. \/ _ \| '__| __|
| |_/ / |_| | |_) | |_) | |  __/ /\__/ / (_) | |  | |_ 
\____/ \__,_|_.__/|_.__/|_|\___| \____/ \___/|_|   \__|
*/

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

/*
 _____      _           _   _               _____            _   
/  ___|    | |         | | (_)             /  ___|          | |  
\ `--.  ___| | ___  ___| |_ _  ___  _ __   \ `--.  ___  _ __| |_ 
 `--. \/ _ \ |/ _ \/ __| __| |/ _ \| '_ \   `--. \/ _ \| '__| __|
/\__/ /  __/ |  __/ (__| |_| | (_) | | | | /\__/ / (_) | |  | |_ 
\____/ \___|_|\___|\___|\__|_|\___/|_| |_| \____/ \___/|_|   \__|
*/

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

/*
 _____                    _   _               _____            _   
|_   _|                  | | (_)             /  ___|          | |  
  | | _ __  ___  ___ _ __| |_ _  ___  _ __   \ `--.  ___  _ __| |_ 
  | || '_ \/ __|/ _ \ '__| __| |/ _ \| '_ \   `--. \/ _ \| '__| __|
 _| || | | \__ \  __/ |  | |_| | (_) | | | | /\__/ / (_) | |  | |_ 
 \___/_| |_|___/\___|_|   \__|_|\___/|_| |_| \____/ \___/|_|   \__|
*/

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

/*
___  ___                      _____            _   
|  \/  |                     /  ___|          | |  
| .  . | ___ _ __ __ _  ___  \ `--.  ___  _ __| |_ 
| |\/| |/ _ \ '__/ _` |/ _ \  `--. \/ _ \| '__| __|
| |  | |  __/ | | (_| |  __/ /\__/ / (_) | |  | |_ 
\_|  |_/\___|_|  \__, |\___| \____/ \___/|_|   \__|
                  __/ |                            
                 |___/                             
*/

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

/*
 _____       _      _      _____            _   
|  _  |     (_)    | |    /  ___|          | |  
| | | |_   _ _  ___| | __ \ `--.  ___  _ __| |_ 
| | | | | | | |/ __| |/ /  `--. \/ _ \| '__| __|
\ \/' / |_| | | (__|   <  /\__/ / (_) | |  | |_ 
 \_/\_\\__,_|_|\___|_|\_\ \____/ \___/|_|   \__|
*/

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

/*
 _   _                    _____            _   
| | | |                  /  ___|          | |  
| |_| | ___  __ _ _ __   \ `--.  ___  _ __| |_ 
|  _  |/ _ \/ _` | '_ \   `--. \/ _ \| '__| __|
| | | |  __/ (_| | |_) | /\__/ / (_) | |  | |_ 
\_| |_/\___|\__,_| .__/  \____/ \___/|_|   \__|
                 | |                           
                 |_|                           
*/

static gboolean heap_phase = TRUE;
static int heap_build_index = 0; // Controls the outer loop of the build phase: (array_size/2)-1 down to 0
static int heap_size = 0;        // Current heap size in the sorting phase
static int heap_i = -1;          // Index used and modified by heapify_single_step_managed.
                                 // Represents the current node in a sift-down path.

// True if we are in the middle of a multi-step sift-down for a single root element.
static gboolean sift_operation_active = FALSE;

gboolean heapify_single_step_managed(int n_heap_bound) {
    // This function directly uses and MODIFIES the global 'heap_i'
    // n_heap_bound is the current effective size of the heap (either array_size or the reduced heap_size)

    // Basic bounds check for heap_i, though logic in heap_sort_step should prevent invalid values.
    if (heap_i < 0 || heap_i >= n_heap_bound) {
        return FALSE; // Should not proceed if heap_i is out of bounds for the current operation
    }

    int left = 2 * heap_i + 1;
    int right = 2 * heap_i + 2;
    int largest = heap_i;

    if (left < n_heap_bound && array[left] > array[largest]) {
        largest = left;
    }
    if (right < n_heap_bound && array[right] > array[largest]) {
        largest = right;
    }

    current_index = heap_i;     // For visualization
    compare_index = largest;    // For visualization

    if (largest != heap_i) {
        // Swap elements
        int temp = array[heap_i];
        array[heap_i] = array[largest];
        array[largest] = temp;

        heap_i = largest; // IMPORTANT: Update global heap_i to continue sifting from the child's position
        return TRUE;      // Indicates that a swap occurred and sifting should continue for this element
    }

    // Element at heap_i is now in its correct heap position relative to its direct children.
    // The current sift-down path initiated for an ancestor (or this node itself) is complete from this node downwards.
    return FALSE;
}

gboolean heap_sort_step(gpointer data) {
    if (stop_requested) {
        for (int k = 0; k < array_size; ++k) {
            sorted_flags[k] = true;
        }
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (heap_phase) { // Build Heap Phase
        if (!sift_operation_active) {
            // Not currently sifting, so decide if we need to start sifting a new node for build-heap
            if (heap_build_index >= 0) {
                heap_i = heap_build_index; // Set the root of the subtree to start sifting
                sift_operation_active = TRUE;
                // The actual sifting will happen in the block below or in the next call to heap_sort_step
            } else {
                // Build heap phase is complete
                heap_phase = FALSE;
                heap_size = array_size;         // Initialize heap_size for sorting phase
                sift_operation_active = FALSE;  // Reset for sorting phase
                // heap_i will be set to 0 by the sorting phase logic when it starts a sift
                // gtk_widget_queue_draw(drawing_area); // Optional: draw after build heap is fully done.
                return TRUE; // Continue to sorting phase logic in the next step
            }
        }

        // This 'if' block can be entered in the same call if sift_operation_active was just set to TRUE above.
        if (sift_operation_active) {
            // Continue sifting the current element (which started at heap_build_index, current node is heap_i)
            gboolean did_sift = heapify_single_step_managed(array_size); // Use full array_size for build-heap
            gtk_widget_queue_draw(drawing_area);

            if (did_sift) {
                // Sifting continues for the current heap_i (which has been updated by heapify_single_step_managed)
                return TRUE;
            } else {
                // Sifting for the element that started at heap_build_index (and was processed via heap_i) is complete
                sift_operation_active = FALSE;
                heap_build_index--; // Move to the next node to heapify in the build phase
                return TRUE;
            }
        }
    } else { // Sorting Phase
        if (heap_size <= 1) {
            if (heap_size == 1) { // Ensure the last element is marked sorted
                sorted_flags[0] = true;
            }
            // gtk_widget_queue_draw(drawing_area); // Draw the final sorted state if needed before message
            show_finished_message();
            return FALSE; // Sorting is complete
        }

        if (!sift_operation_active) {
            // Not currently sifting, so perform swap and then prepare for the next sift-down.
            // Swap the root (maximum element) with the last element of the current heap
            int temp = array[0];
            array[0] = array[heap_size - 1];
            array[heap_size - 1] = temp;
            sorted_flags[heap_size - 1] = true; // Mark the swapped element as sorted
            heap_size--; // Reduce the heap size

            // Check if sorting is complete after this swap and reduction
            if (heap_size <= 1) {
                if (heap_size == 1) {
                    sorted_flags[0] = true;
                }
                gtk_widget_queue_draw(drawing_area); // Show the swap and potential final element
                show_finished_message();
                return FALSE;
            }

            heap_i = 0; // Set heap_i to the root to start sifting for the reduced heap
            sift_operation_active = TRUE;
            gtk_widget_queue_draw(drawing_area); // Show the state after the swap
            return TRUE; // Next step will start sifting this new root
        }

        // This 'if' block can be entered in the same call if sift_operation_active was just set to TRUE above.
        if (sift_operation_active) {
            // Continue sifting the root element (current node is heap_i, which started at 0)
            gboolean did_sift = heapify_single_step_managed(heap_size); // Use current (reduced) heap_size
            gtk_widget_queue_draw(drawing_area);

            if (did_sift) {
                // Sifting continues for the current heap_i (which has been updated)
                return TRUE;
            } else {
                // Sifting for the root (which started at 0) is complete
                sift_operation_active = FALSE;
                // Ready for the next swap in the sort phase (will be handled in the next call to heap_sort_step)
                return TRUE;
            }
        }
    }
    return TRUE;
}

void reset_heap_sort() {
    heap_phase = TRUE;
    // Start building the heap from the last non-leaf node
    heap_build_index = (array_size / 2) - 1;
    heap_size = 0; // Will be set to array_size when sort phase begins
    heap_i = -1;   // Indicates heap_i is not yet set for an operation
    sift_operation_active = FALSE;

    current_index = -1; // Reset visualization index
    compare_index = -1; // Reset visualization index

    for (int k = 0; k < array_size; ++k) {
        sorted_flags[k] = false;
    }

    gtk_widget_queue_draw(drawing_area);
}

/*
 _____                   _     _____            _   
/  __ \                 | |   /  ___|          | |  
| /  \/ ___  _   _ _ __ | |_  \ `--.  ___  _ __| |_ 
| |    / _ \| | | | '_ \| __|  `--. \/ _ \| '__| __|
| \__/\ (_) | |_| | | | | |_  /\__/ / (_) | |  | |_ 
 \____/\___/ \__,_|_| |_|\__| \____/ \___/|_|   \__|
*/

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

/*
______          _ _        _____            _   
| ___ \        | (_)      /  ___|          | |  
| |_/ /__ _  __| |___  __ \ `--.  ___  _ __| |_ 
|    // _` |/ _` | \ \/ /  `--. \/ _ \| '__| __|
| |\ \ (_| | (_| | |>  <  /\__/ / (_) | |  | |_ 
\_| \_\__,_|\__,_|_/_/\_\ \____/ \___/|_|   \__|
*/

static int radix_exp = 1;

gboolean radix_sort_step(gpointer data) {
    if (stop_requested || radix_exp > 1000000000) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    int* output = (int*)malloc(array_size * sizeof(int));
    int count[20] = {0}; // Use count array of size 20 to handle negative numbers

    if (!output) {
        fprintf(stderr, "Memory allocation failed in radix_sort_step\n");
        return FALSE;
    }

    // 1. Count occurrences of digits (shift negative digits to positive indices in count)
    for (int i = 0; i < array_size; i++) {
        int digit = (array[i] / radix_exp) % 10;
        count[digit + 10]++; // Shift by 10 to handle negative digits
    }

    // 2. Compute cumulative counts
    for (int i = 1; i < 20; i++)
        count[i] += count[i - 1];

    // 3. Build the output array (stable sorting)
    for (int i = array_size - 1; i >= 0; i--) {
        int digit = (array[i] / radix_exp) % 10;
        output[count[digit + 10] - 1] = array[i]; // Shift by 10 again
        count[digit + 10]--;
    }

    // 4. Copy the output back to the original array
    for (int i = 0; i < array_size; i++)
        array[i] = output[i];

    // 5. Increment the radix_exponent
    radix_exp *= 10;

    // 6. Trigger redraw and free memory
    gtk_widget_queue_draw(drawing_area);
    free(output);
    return TRUE;
}

void reset_radix_sort() {
    radix_exp = 1;
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;
    gtk_widget_queue_draw(drawing_area);
}


/*
______            _        _     _____            _   
| ___ \          | |      | |   /  ___|          | |  
| |_/ /_   _  ___| | _____| |_  \ `--.  ___  _ __| |_ 
| ___ \ | | |/ __| |/ / _ \ __|  `--. \/ _ \| '__| __|
| |_/ / |_| | (__|   <  __/ |_  /\__/ / (_) | |  | |_ 
\____/ \__,_|\___|_|\_\___|\__| \____/ \___/|_|   \__|
*/

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

/*
 _____ _          _ _   _____            _   
/  ___| |        | | | /  ___|          | |  
\ `--.| |__   ___| | | \ `--.  ___  _ __| |_ 
 `--. \ '_ \ / _ \ | |  `--. \/ _ \| '__| __|
/\__/ / | | |  __/ | | /\__/ / (_) | |  | |_ 
\____/|_| |_|\___|_|_| \____/ \___/|_|   \__|
*/

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

/*
 _____ _             _____            _   
|_   _(_)           /  ___|          | |  
  | |  _ _ __ ___   \ `--.  ___  _ __| |_ 
  | | | | '_ ` _ \   `--. \/ _ \| '__| __|
  | | | | | | | | | /\__/ / (_) | |  | |_ 
  \_/ |_|_| |_| |_| \____/ \___/|_|   \__|
*/

static int min_run = 0;

// Variables for insertion sort state
static gboolean insertion_phase = FALSE;
static int current_run_start = 0;
static int insert_i = 0;
static int insert_j = 0;
static int insert_key = 0;
static gboolean insert_in_progress = FALSE;

// Variables for merge phase state
static int current_merge_size = 0;
static int current_merge_left = 0;
static int current_merge_mid = 0;
static int current_merge_right = 0;
static int temp_left_ptr = 0;
static int temp_right_ptr = 0;
static int array_write_ptr = 0;
static gboolean merging_in_progress = FALSE;

static int segment_start_left = 0;

static int *temp_array = NULL;

gboolean tim_sort_step(gpointer data) {
    if (stop_requested) {
        gtk_widget_queue_draw(drawing_area);
        return FALSE;
    }

    // Initialization
    if (min_run == 0) {
        int n = array_size;
        int r = 0;
        while (n >= 64) {
            r |= (n & 1);
            n >>= 1;
        }
        min_run = n + r;
        if (min_run < 1) min_run = 1;

        if (temp_array) free(temp_array);
        temp_array = (int*)malloc(array_size * sizeof(int));
        if (!temp_array) {
            fprintf(stderr, "Failed to allocate temporary array for Timsort.\n");
            reset_tim_sort();
            return FALSE;
        }

        insertion_phase = TRUE;
        current_run_start = 0;
        insert_in_progress = FALSE;

        current_merge_size = 0;
        segment_start_left = 0;
        merging_in_progress = FALSE;

        current_index = -1;
        compare_index = -1;

        if (sorted_flags && array_size > 0) {
            for (int k = 0; k < array_size; ++k) {
                sorted_flags[k] = false;
            }
        }

        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    }

    // Insertion phase
    if (insertion_phase) {
        if (current_run_start >= array_size) {
            insertion_phase = FALSE;
            current_merge_size = min_run;
            segment_start_left = 0;
            merging_in_progress = FALSE;

            insert_i = 0;
            insert_j = 0;
            insert_key = 0;
            insert_in_progress = FALSE;
            current_run_start = 0;

            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }

        int run_end = (current_run_start + min_run < array_size) ? current_run_start + min_run : array_size;

        if (!insert_in_progress) {
            insert_i = current_run_start + 1;
            while (insert_i < run_end && array[insert_i - 1] <= array[insert_i]) {
                insert_i++;
            }

            if (insert_i >= run_end) {
                current_run_start = run_end;
                gtk_widget_queue_draw(drawing_area);
                return TRUE;
            }

            insert_key = array[insert_i]; 
            insert_j = insert_i - 1;
            insert_in_progress = TRUE;

            current_index = insert_i;
            compare_index = insert_j;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }

        if (insert_j >= current_run_start && array[insert_j] > insert_key) {
            array[insert_j + 1] = array[insert_j];
            insert_j--;

            current_index = insert_j + 1;
            compare_index = insert_j;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        } else {
            array[insert_j + 1] = insert_key;

            current_index = insert_j + 1;
            compare_index = -1;

            insert_in_progress = FALSE;

            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }
    }

    // Merge phase
    if (current_merge_size >= array_size) {
        for (int k = 0; k < array_size; ++k) {
            sorted_flags[k] = true;
        }
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();

        if (temp_array) {
            free(temp_array);
            temp_array = NULL;
        }

        return FALSE;
    }

    if (!merging_in_progress) {
        if (segment_start_left >= array_size) {
            current_merge_size *= 2;
            segment_start_left = 0;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }

        int mid_point = segment_start_left + current_merge_size;
        int end_right = segment_start_left + 2 * current_merge_size;
        if (end_right > array_size) end_right = array_size;
        if (mid_point < end_right) {
            current_merge_left = segment_start_left;
            current_merge_mid = mid_point;
            current_merge_right = end_right;

            merging_in_progress = TRUE;

            int segment_length = current_merge_right - current_merge_left;
            for (int i = 0; i < segment_length; i++) {
                temp_array[i] = array[current_merge_left + i];
            }

            // *** FIX: Initialize the left pointer for merging ***
            temp_left_ptr = 0;
            temp_right_ptr = current_merge_mid - current_merge_left;
            array_write_ptr = current_merge_left;

            segment_start_left = end_right;

            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        } else {
            segment_start_left = end_right;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }
    } else {
        int mid_boundary_in_temp = current_merge_mid - current_merge_left;
        int right_boundary_in_temp = current_merge_right - current_merge_left;

        if (temp_left_ptr < mid_boundary_in_temp &&
            (temp_right_ptr >= right_boundary_in_temp || temp_array[temp_left_ptr] <= temp_array[temp_right_ptr])) {
            array[array_write_ptr] = temp_array[temp_left_ptr];
            temp_left_ptr++;
        } else if (temp_right_ptr < right_boundary_in_temp) {
            array[array_write_ptr] = temp_array[temp_right_ptr];
            temp_right_ptr++;
        }

        current_index = array_write_ptr;
        compare_index = -1;

        array_write_ptr++;

        if (array_write_ptr >= current_merge_right) {
            merging_in_progress = FALSE;
        }

        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    }
}

void reset_tim_sort() {
    min_run = 0;

    insertion_phase = FALSE;
    current_run_start = 0;
    insert_i = 0;
    insert_j = 0;
    insert_key = 0;
    insert_in_progress = FALSE;

    current_merge_size = 0;
    current_merge_left = 0;
    current_merge_mid = 0;
    current_merge_right = 0;
    temp_left_ptr = 0;
    temp_right_ptr = 0;
    array_write_ptr = 0;
    merging_in_progress = FALSE;
    segment_start_left = 0;

    if (temp_array) {
        free(temp_array);
        temp_array = NULL;
    }

    current_index = -1;
    compare_index = -1;

    if (sorted_flags && array_size > 0) {
        for (int k = 0; k < array_size; ++k) {
            sorted_flags[k] = false;
        }
    }
    gtk_widget_queue_draw(drawing_area);
}

/*
 _____      _               _____            _   
|_   _|    | |             /  ___|          | |  
  | | _ __ | |_ _ __ ___   \ `--.  ___  _ __| |_ 
  | || '_ \| __| '__/ _ \   `--. \/ _ \| '__| __|
 _| || | | | |_| | | (_) | /\__/ / (_) | |  | |_ 
 \___/_| |_|\__|_|  \___/  \____/ \___/|_|   \__|
*/

static int depth_limit;
static int pivot_index = -1;
static int partition_left = 0, partition_right = 0;
static int heap_start = 0, heap_end = 0;
static gboolean quick_sort_phase = FALSE;
static gboolean heap_sort_phase = FALSE;
static gboolean insertion_sort_phase = FALSE;
static gboolean partitioning = FALSE;
static gboolean heapifying = FALSE;
static int* stack = NULL;
static int stack_top = -1;
static int stack_size = 0;

static void push_range(int left, int right) {
    if (stack_top + 2 >= stack_size) {
        stack_size = (stack_size == 0) ? 32 : stack_size * 2;
        stack = realloc(stack, stack_size * sizeof(int));
        if (!stack) return;
    }
    stack[++stack_top] = left;
    stack[++stack_top] = right;
}

static gboolean pop_range(int* left, int* right) {
    if (stack_top < 1) return FALSE;
    *right = stack[stack_top--];
    *left = stack[stack_top--];
    return TRUE;
}

gboolean intro_sort_step(gpointer data) {
    if (stop_requested) {
        gtk_widget_queue_draw(drawing_area);
        return FALSE;
    }

    // Initialization phase
    if (depth_limit == 0) {
        depth_limit = 2 * log2(array_size);
        if (stack) free(stack);
        stack = NULL;
        stack_top = -1;
        stack_size = 0;
        
        push_range(0, array_size - 1);
        quick_sort_phase = TRUE;
        return TRUE;
    }

    // Quick sort phase
    if (quick_sort_phase) {
        if (!partitioning) {
            if (!pop_range(&partition_left, &partition_right)) {
                // No more ranges from quicksort; perform final insertion sort pass
                partition_left = 0;
                partition_right = array_size - 1;
                quick_sort_phase = FALSE;
                insertion_sort_phase = TRUE;
                return TRUE;
            }
            
            if (partition_right - partition_left + 1 <= 16) {
                // Range is small, push into insertion sort phase
                push_range(partition_left, partition_right);
                quick_sort_phase = FALSE;
                insertion_sort_phase = TRUE;
                return TRUE;
            }
            
            if (depth_limit <= 0) {
                // Switch to heap sort if recursion depth is exceeded
                heap_sort_phase = TRUE;
                quick_sort_phase = FALSE;
                heap_start = 0;
                heap_end = partition_right - partition_left;
                heapifying = TRUE;
                return TRUE;
            }
            
            // Choose pivot (median of three)
            int mid = partition_left + (partition_right - partition_left) / 2;
            if (array[partition_left] > array[mid]) {
                int temp = array[partition_left];
                array[partition_left] = array[mid];
                array[mid] = temp;
            }
            if (array[partition_left] > array[partition_right]) {
                int temp = array[partition_left];
                array[partition_left] = array[partition_right];
                array[partition_right] = temp;
            }
            if (array[mid] > array[partition_right]) {
                int temp = array[mid];
                array[mid] = array[partition_right];
                array[partition_right] = temp;
            }
            
            // Place pivot at partition_right - 1
            int temp = array[mid];
            array[mid] = array[partition_right - 1];
            array[partition_right - 1] = temp;
            
            pivot_index = partition_right - 1;
            partitioning = TRUE;
            return TRUE;
        } else {
            // Partitioning step:
            int i = partition_left;
            int j = partition_right - 1;
            while (1) {
                while (array[++i] < array[pivot_index]);
                while (array[--j] > array[pivot_index]);
                
                if (i < j) {
                    // Swap elements
                    int temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                    
                    // Update visualization
                    current_index = i;
                    compare_index = j;
                    gtk_widget_queue_draw(drawing_area);
                } else {
                    break;
                }
            }
            
            // Swap pivot into place
            int temp = array[i];
            array[i] = array[pivot_index];
            array[pivot_index] = temp;
            
            // Update visualization
            current_index = i;
            compare_index = pivot_index;
            gtk_widget_queue_draw(drawing_area);

            
            // Push the larger partition first
            if (i - partition_left > partition_right - i) {
                push_range(partition_left, i - 1);
                push_range(i + 1, partition_right);
            } else {
                push_range(i + 1, partition_right);
                push_range(partition_left, i - 1);
            }
            
            depth_limit--;
            partitioning = FALSE;
            return TRUE;
        }
    }

    // Heap sort phase
    if (heap_sort_phase) {
        if (heapifying) {
            int largest = heap_start;
            int left_child = 2 * heap_start + 1;
            int right_child = 2 * heap_start + 2;
            
            if (left_child <= heap_end && array[partition_left + left_child] > array[partition_left + largest]) {
                largest = left_child;
            }
            if (right_child <= heap_end && array[partition_left + right_child] > array[partition_left + largest]) {
                largest = right_child;
            }
            if (largest != heap_start) {
                // Swap elements
                int temp = array[partition_left + heap_start];
                array[partition_left + heap_start] = array[partition_left + largest];
                array[partition_left + largest] = temp;
                
                current_index = partition_left + heap_start;
                compare_index = partition_left + largest;
                gtk_widget_queue_draw(drawing_area);
                
                heap_start = largest;
                return TRUE;
            } else {
                heapifying = FALSE;
                return TRUE;
            }
        } else {
            if (heap_end > 0) {
                // Swap root with last element of heap
                int temp = array[partition_left];
                array[partition_left] = array[partition_left + heap_end];
                array[partition_left + heap_end] = temp;
                
                current_index = partition_left;
                compare_index = partition_left + heap_end;
                gtk_widget_queue_draw(drawing_area);
                
                heap_end--;
                heap_start = 0;
                heapifying = TRUE;
                return TRUE;
            } else {
                // Heap sort complete for this range; move on to final insertion sort pass
                heap_sort_phase = FALSE;
                insertion_sort_phase = TRUE;
                return TRUE;
            }
        }
    }

    // Insertion phase
    if (insertion_sort_phase) {
        if (!pop_range(&partition_left, &partition_right)) {
            // Sorting complete
            for (int k = 0; k < array_size; ++k)
                sorted_flags[k] = true;
            gtk_widget_queue_draw(drawing_area);
            show_finished_message();
            return FALSE;
        }
        
        if (partition_right - partition_left + 1 <= 16) {
            // Perform insertion sort on small range
            for (int i = partition_left + 1; i <= partition_right; i++) {
                int key = array[i];
                int j = i - 1;
                
                while (j >= partition_left && array[j] > key) {
                    array[j + 1] = array[j];
                    j--;
                    
                    // Update visualization
                    current_index = j + 1;
                    compare_index = j;
                    gtk_widget_queue_draw(drawing_area);
                }
                array[j + 1] = key;
            }
            return TRUE;
        } else {
            for (int i = partition_left + 1; i <= partition_right; i++) {
                int key = array[i];
                int j = i - 1;
                while (j >= partition_left && array[j] > key) {
                    array[j + 1] = array[j];
                    j--;
                    current_index = j + 1;
                    compare_index = j;
                    gtk_widget_queue_draw(drawing_area);
                }
                array[j + 1] = key;
            }
            return TRUE;
        }
    }
    return FALSE;
}

void reset_intro_sort() {
    depth_limit = 0;
    pivot_index = -1;
    partition_left = partition_right = 0;
    heap_start = heap_end = 0;
    quick_sort_phase = FALSE;
    heap_sort_phase = FALSE;
    insertion_sort_phase = FALSE;
    partitioning = FALSE;
    heapifying = FALSE;
    
    if (stack) {
        free(stack);
        stack = NULL;
    }
    stack_top = -1;
    stack_size = 0;
    
    current_index = compare_index = -1;
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;
    gtk_widget_queue_draw(drawing_area);
}

/*
 _____            _        _____            _   
/  __ \          | |      /  ___|          | |  
| /  \/_   _  ___| | ___  \ `--.  ___  _ __| |_ 
| |   | | | |/ __| |/ _ \  `--. \/ _ \| '__| __|
| \__/\ |_| | (__| |  __/ /\__/ / (_) | |  | |_ 
 \____/\__, |\___|_|\___| \____/ \___/|_|   \__|
        __/ |                                   
       |___/                                    
*/

// Cycle Sort State Variables
static int cycle_start = 0;
static int cycle_position = 0;
static int cycle_value = 0;
static int cycle_writes = 0;
static gboolean in_cycle = FALSE;

gboolean cycle_sort_step(gpointer data) {
    if (stop_requested) {
        gtk_widget_queue_draw(drawing_area);
        return FALSE;
    }

    if (!in_cycle) {
        if (cycle_start >= array_size - 1) {
            for (int k = 0; k < array_size; ++k)
                sorted_flags[k] = true;
            gtk_widget_queue_draw(drawing_area);
            show_finished_message();
            return FALSE;
        }

        cycle_value = array[cycle_start];
        cycle_position = cycle_start;

        for (int i = cycle_start + 1; i < array_size; i++) {
            if (array[i] < cycle_value)
                cycle_position++;
        }

        if (cycle_position == cycle_start) {
            cycle_start++;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }

        while (cycle_position < array_size && array[cycle_position] == cycle_value) {
            cycle_position++;
        }

        if (cycle_position >= array_size) {
            cycle_start++;
            current_index = compare_index = -1;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }

        in_cycle = TRUE;
        cycle_writes = 0;

        // Perform first swap
        int temp = array[cycle_position];
        array[cycle_position] = cycle_value;
        cycle_value = temp;
        cycle_writes++;

        current_index = cycle_start;
        compare_index = cycle_position;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    } else {
        // Continue the cycle
        cycle_position = cycle_start;

        for (int i = cycle_start + 1; i < array_size; i++) {
            if (array[i] < cycle_value)
                cycle_position++;
        }

        while (cycle_position < array_size && array[cycle_position] == cycle_value) {
            cycle_position++;
        }

        if (cycle_position >= array_size) {
            in_cycle = FALSE;
            cycle_start++;
            current_index = compare_index = -1;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }

        // Perform swap
        int temp = array[cycle_position];
        array[cycle_position] = cycle_value;
        cycle_value = temp;
        cycle_writes++;

        // If placed in correct spot, cycle is done
        if (cycle_position == cycle_start) {
            in_cycle = FALSE;
            cycle_start++;
        }

        current_index = cycle_start;
        compare_index = cycle_position;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;
    }

    return TRUE;
}

// Reset Function
void reset_cycle_sort() {
    cycle_start = 0;
    cycle_position = 0;
    cycle_value = 0;
    cycle_writes = 0;
    in_cycle = FALSE;
    current_index = compare_index = -1;
    
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;
    
    gtk_widget_queue_draw(drawing_area);
}

/*
 _____            _    _        _ _   _____ _           _               _____            _   
/  __ \          | |  | |      (_) | /  ___| |         | |             /  ___|          | |  
| /  \/ ___   ___| | _| |_ __ _ _| | \ `--.| |__   __ _| | _____ _ __  \ `--.  ___  _ __| |_ 
| |    / _ \ / __| |/ / __/ _` | | |  `--. \ '_ \ / _` | |/ / _ \ '__|  `--. \/ _ \| '__| __|
| \__/\ (_) | (__|   <| || (_| | | | /\__/ / | | | (_| |   <  __/ |    /\__/ / (_) | |  | |_ 
 \____/\___/ \___|_|\_\\__\__,_|_|_| \____/|_| |_|\__,_|_|\_\___|_|    \____/ \___/|_|   \__|
*/

// Cocktail Shaker Sort State Variables
static int start = 0;
static int end = 0;
static int current = 0;
static gboolean forward_pass = TRUE;
static gboolean swapped_in_pass = FALSE;

// Cocktail Shaker Step Function
gboolean cocktail_shaker_sort_step(gpointer data) {
    if (stop_requested) {
        reset_cocktail_shaker_sort();
        gtk_widget_queue_draw(drawing_area);
        return FALSE;
    }

    // Sorting complete when no swaps occur in a full pass
    if (start >= end) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    if (forward_pass) {
        // Forward pass (left to right)
        if (current < end) {
            if (array[current] > array[current + 1]) {
                // Swap elements
                int temp = array[current];
                array[current] = array[current + 1];
                array[current + 1] = temp;
                swapped_in_pass = TRUE;

                // Update visualization
                current_index = current;
                compare_index = current + 1;
            } else {
                // Just compare (no swap)
                current_index = current;
                compare_index = current + 1;
            }
            current++;
        } else {
            // End of forward pass
            if (!swapped_in_pass) {
                // Early exit if no swaps occurred
                end = start;
            } else {
                end--;
                current = end;
                forward_pass = FALSE;
                swapped_in_pass = FALSE;
            }
        }
    } else {
        // Backward pass (right to left)
        if (current > start) {
            if (array[current] < array[current - 1]) {
                // Swap elements
                int temp = array[current];
                array[current] = array[current - 1];
                array[current - 1] = temp;
                swapped_in_pass = TRUE;

                // Update visualization
                current_index = current;
                compare_index = current - 1;
            } else {
                // Just compare (no swap)
                current_index = current;
                compare_index = current - 1;
            }
            current--;
        } else {
            // End of backward pass
            if (!swapped_in_pass) {
                // Early exit if no swaps occurred
                start = end;
            } else {
                start++;
                current = start;
                forward_pass = TRUE;
                swapped_in_pass = FALSE;
            }
        }
    }

    gtk_widget_queue_draw(drawing_area);
    return TRUE;
}

// Reset Function
void reset_cocktail_shaker_sort() {
    start = 0;
    end = array_size - 1;
    current = 0;
    forward_pass = TRUE;
    swapped_in_pass = FALSE;
    current_index = compare_index = -1;

    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;

    gtk_widget_queue_draw(drawing_area);
}

/*
 _____                              _____            _   
|  __ \                            /  ___|          | |  
| |  \/_ __   ___  _ __ ___   ___  \ `--.  ___  _ __| |_ 
| | __| '_ \ / _ \| '_ ` _ \ / _ \  `--. \/ _ \| '__| __|
| |_\ \ | | | (_) | | | | | |  __/ /\__/ / (_) | |  | |_ 
 \____/_| |_|\___/|_| |_| |_|\___| \____/ \___/|_|   \__|
*/

static int gnome_pos = 0;

gboolean gnome_sort_step(gpointer data) {
    if (stop_requested) {
        gtk_widget_queue_draw(drawing_area);
        return FALSE;
    }

    if (gnome_pos == 0) gnome_pos++;

    // Sorting complete
    if (gnome_pos >= array_size) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    // Compare and swap if needed
    current_index = gnome_pos - 1;
    compare_index = gnome_pos;

    if (array[gnome_pos - 1] > array[gnome_pos]) {
        // Swap
        int temp = array[gnome_pos];
        array[gnome_pos] = array[gnome_pos - 1];
        array[gnome_pos - 1] = temp;
        
        // Move backward
        if (gnome_pos > 1) gnome_pos--;
    } else {
        // Move forward
        gnome_pos++;
    }

    gtk_widget_queue_draw(drawing_area);
    return TRUE;
}

void reset_gnome_sort() {
    gnome_pos = 0;
    current_index = compare_index = -1;
    
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;
    
    gtk_widget_queue_draw(drawing_area);
}

/*
 _____                 _       _____            _   
/  __ \               | |     /  ___|          | |  
| /  \/ ___  _ __ ___ | |__   \ `--.  ___  _ __| |_ 
| |    / _ \| '_ ` _ \| '_ \   `--. \/ _ \| '__| __|
| \__/\ (_) | | | | | | |_) | /\__/ / (_) | |  | |_ 
 \____/\___/|_| |_| |_|_.__/  \____/ \___/|_|   \__|
*/

static int comb_gap = 0;
static int comb_i = 0;
static gboolean comb_swapped = FALSE;
static float shrink_factor = 1.3; // Optimal shrink factor

gboolean comb_sort_step(gpointer data) {
    if (stop_requested) {
        gtk_widget_queue_draw(drawing_area);
        return FALSE;
    }

    // Initialize gap if first time
    if (comb_gap == 0) {
        comb_gap = array_size;
        comb_i = 0;
        comb_swapped = FALSE;
        return TRUE;
    }

    // If still within bounds for a single step in the current pass
    if (comb_i + comb_gap < array_size) {
        current_index = comb_i;
        compare_index = comb_i + comb_gap;

        if (array[comb_i] > array[comb_i + comb_gap]) {
            int temp = array[comb_i];
            array[comb_i] = array[comb_i + comb_gap];
            array[comb_i + comb_gap] = temp;
            comb_swapped = TRUE;
        }

        comb_i++;
        gtk_widget_queue_draw(drawing_area);
        return TRUE;  // Wait for next GTK timeout
    }

    // If pass is complete, prepare next gap & reset for next pass
    if (comb_gap > 1 || comb_swapped) {
        comb_gap = (int)(comb_gap / shrink_factor);
        if (comb_gap < 1) comb_gap = 1;

        comb_i = 0;
        comb_swapped = FALSE;

        return TRUE;
    }

    // Sorting is complete (gap == 1 && !comb_swapped)
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = true;

    current_index = compare_index = -1;
    gtk_widget_queue_draw(drawing_area);
    show_finished_message();
    return FALSE;
}

void reset_comb_sort() {
    comb_gap = 0;
    comb_i = 0;
    comb_swapped = FALSE;
    current_index = compare_index = -1;

    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;

    gtk_widget_queue_draw(drawing_area);
}

/*
______                     _____            _   
| ___ \                   /  ___|          | |  
| |_/ / ___   __ _  ___   \ `--.  ___  _ __| |_ 
| ___ \/ _ \ / _` |/ _ \   `--. \/ _ \| '__| __|
| |_/ / (_) | (_| | (_) | /\__/ / (_) | |  | |_ 
\____/ \___/ \__, |\___/  \____/ \___/|_|   \__|
              __/ |                             
             |___/                              
*/

static gboolean is_shuffling = FALSE;
static int shuffle_pos = 0;
static int shuffle_remaining = 0;
static int *shuffle_copy = NULL;

static gboolean is_sorted() {
    for (int i = 0; i < array_size - 1; i++) {
        if (array[i] > array[i + 1]) {
            return FALSE;
        }
    }
    return TRUE;
}

gboolean bogo_sort_step(gpointer data) {
    if (stop_requested) {
        reset_bogo_sort();
        gtk_widget_queue_draw(drawing_area);
        return FALSE;
    }

    // Check if sorted (miraculously)
    if (is_sorted()) {
        for (int k = 0; k < array_size; ++k)
            sorted_flags[k] = true;
        gtk_widget_queue_draw(drawing_area);
        show_finished_message();
        return FALSE;
    }

    // Visualize the shuffle step-by-step
    if (!is_shuffling) {
        // Begin new shuffle
        is_shuffling = TRUE;
        shuffle_pos = 0;
        shuffle_remaining = array_size;
        
        // Create a copy for visualization
        if (shuffle_copy) free(shuffle_copy);
        shuffle_copy = (int*)malloc(array_size * sizeof(int));
        memcpy(shuffle_copy, array, array_size * sizeof(int));
        
        return TRUE;
    } else {
        // Perform one step of Fisher-Yates shuffle
        if (shuffle_remaining > 1) {
            int j = rand() % shuffle_remaining;
            
            // Swap elements
            int temp = array[shuffle_pos];
            array[shuffle_pos] = array[j];
            array[j] = temp;
            
            // Update visualization
            current_index = shuffle_pos;
            compare_index = j;
            shuffle_pos++;
            shuffle_remaining--;
            
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        } else {
            // Shuffle complete
            is_shuffling = FALSE;
            free(shuffle_copy);
            shuffle_copy = NULL;
            
            // Check if sorted in next step
            current_index = -1;
            compare_index = -1;
            gtk_widget_queue_draw(drawing_area);
            return TRUE;
        }
    }
}

void reset_bogo_sort() {
    is_shuffling = FALSE;
    shuffle_pos = 0;
    shuffle_remaining = 0;
    
    if (shuffle_copy) {
        free(shuffle_copy);
        shuffle_copy = NULL;
    }
    
    current_index = compare_index = -1;
    
    for (int k = 0; k < array_size; ++k)
        sorted_flags[k] = false;
    
    gtk_widget_queue_draw(drawing_area);
}

/* Helper Funt to reset everything */
void reset_sort() {
    reset_merge_sort();
    reset_quick_sort();
    reset_heap_sort();
    reset_counting_sort();
    reset_radix_sort();
    reset_bucket_sort();
    reset_shell_sort();
    reset_tim_sort();
    reset_intro_sort();
    reset_cycle_sort();
    reset_cocktail_shaker_sort();
    reset_gnome_sort();
    reset_comb_sort();
    reset_bogo_sort();
}