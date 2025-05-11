#ifndef ALGORITMS_H
#define ALGORITMS_H

#define MAX_STACK_SIZE 100

typedef struct {
    int left;
    int right;
} StackItem;

gboolean bubble_sort_step(gpointer data); 
gboolean selection_sort_step(gpointer data);
gboolean insert_sort_step(gpointer data);
gboolean merge_sort_step(gpointer data);
void reset_merge_sort();
gboolean quick_sort_step(gpointer data);
void reset_quick_sort();
gboolean heap_sort_step(gpointer data);
gboolean couting_sort_step(gpointer data);
gboolean radix_sort_step(gpointer data);
gboolean bucket_sort_step(gpointer data);
gboolean shell_sort_step(gpointer data);

void reset_sort();

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
*/


#endif
