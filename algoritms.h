#ifndef ALGORITMS_H
#define ALGORITMS_H

#define VALUE_RANGE (MAX_VALUE - MIN_VALUE + 1)
#define OFFSET (-MIN_VALUE)
#define MAX_STACK_SIZE 64

#define BUCKETS 10

typedef struct {
    int left;
    int right;
} QuickFrame;

gboolean bubble_sort_step(gpointer data); 
gboolean selection_sort_step(gpointer data);
gboolean insert_sort_step(gpointer data);
gboolean merge_sort_step(gpointer data);
gboolean quick_sort_step(gpointer data);
gboolean heap_sort_step(gpointer data);
gboolean counting_sort_step(gpointer data);
gboolean radix_sort_step(gpointer data);
gboolean bucket_sort_step(gpointer data);
gboolean shell_sort_step(gpointer data);

void push_quick(int left, int right);
gboolean pop_quick(int *left, int *right);
gboolean heapify_single_step_managed(int n_heap_bound);
void reset_merge_sort();
void reset_quick_sort();
void reset_heap_sort();
void reset_counting_sort();
void reset_radix_sort();
void reset_shell_sort();
void reset_bucket_sort();
void reset_sort();

void reset_bucket_sort();
int int_compare(const void* a, const void* b);
void initialize_buckets();

void reset_tim_sort();
gboolean tim_sort_step(gpointer data);
gboolean intro_sort_step(gpointer data);
void reset_intro_sort();

gboolean cycle_sort_step(gpointer data);
void reset_cycle_sort();
gboolean cocktail_shaker_sort_step(gpointer data);
void reset_cocktail_shaker_sort();
gboolean gnome_sort_step(gpointer data);
void reset_gnome_sort();
gboolean comb_sort_step(gpointer data);
void reset_comb_sort();
gboolean bogo_sort_step(gpointer data);
void reset_bogo_sort();

#endif

/*
1.2.1 Bubble Sort
1.2.2 Insertion Sort
1.2.3 Selection Sort
1.2.4 Merge Sort
1.2.5 Quick Sort
1.2.6 Heap Sort
1.2.7 Counting Sort
1.2.8 Radix Sort
1.2.9 Bucket Sort
1.2.10 Shell Sort
1.2.11 Tim Sort
1.2.12 Intro Sort
1.2.13 Cycle Sort
1.2.14 Cocktail shaker Sort
1.2.15 Gnome Sort
1.2.16 Comb Sort
1.2.17 Bogo Sort
*/