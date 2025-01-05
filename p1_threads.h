#ifndef __P1_THREADS
#define __P1_THREADS

#include <vector>
#include <pthread.h>

#include "p1_process.h"

// Class to handle multithreaded merge sort
class ParallelMergeSorter {
  private:
    std::vector<pthread_t> threads;
    std::vector<student> sorted_list;
    int num_threads;

    static void * thread_init(void *);

    void merge_sort(int, int);
    void merge(int, int, int);
    void merge_threads();
  public:
    ParallelMergeSorter(std::vector<student> &, int);
    void merge_sorted_sublists(const std::vector<student>& list, int start, int end, std::vector<student>& merged_list);

    std::vector<student> run_sort();
};

#endif
