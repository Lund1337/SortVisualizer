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

static int exp = 1;

gboolean radix_sort_step(gpointer data) {
    if (stop_requested || exp > 1000000000) {
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
        int digit = (array[i] / exp) % 10;
        count[digit + 10]++; // Shift by 10 to handle negative digits
    }

    // 2. Compute cumulative counts
    for (int i = 1; i < 20; i++)
        count[i] += count[i - 1];

    // 3. Build the output array (stable sorting)
    for (int i = array_size - 1; i >= 0; i--) {
        int digit = (array[i] / exp) % 10;
        output[count[digit + 10] - 1] = array[i]; // Shift by 10 again
        count[digit + 10]--;
    }

    // 4. Copy the output back to the original array
    for (int i = 0; i < array_size; i++)
        array[i] = output[i];

    // 5. Increment the exponent
    exp *= 10;

    // 6. Trigger redraw and free memory
    gtk_widget_queue_draw(drawing_area);
    free(output);
    return TRUE;
}

void reset_radix_sort() {
    exp = 1;
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

/* Helper Funt to reset everything */
void reset_sort() {
    reset_merge_sort();
    reset_quick_sort();
    reset_heap_sort();
    reset_counting_sort();
    reset_radix_sort();
    reset_bucket_sort();
    reset_shell_sort();
}