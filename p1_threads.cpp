#include <vector>
#include <pthread.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <unistd.h>

#include "p1_process.h"
#include "p1_threads.h"

using namespace std;

// Struct to pass arguments into each thread
struct MergeSortArgs {
    int thread_index;
    ParallelMergeSorter *ctx;

    MergeSortArgs(ParallelMergeSorter *ctx, int thread_index) {
        this->ctx = ctx;
        this->thread_index = thread_index;
    }
};

// Class constructor
ParallelMergeSorter::ParallelMergeSorter(vector<student> &original_list, int num_threads) {
    sorted_list = original_list;
    this->num_threads = num_threads;
}

// This function will be called by each child process to perform multithreaded sorting
vector<student> ParallelMergeSorter::run_sort() {
    for (int i = 0; i < num_threads; i++) {
        MergeSortArgs *args = new MergeSortArgs(this, i);
        pthread_t thread;
        pthread_create(&thread, NULL, thread_init, args);

        //used to keep track of threads
        threads.push_back(thread);
    }

    //when all threads are done with the sorting process, we
    //need to join them together 
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

  //sublist merging
    merge_threads();

  //return the sorted list to be used in p1_process
    return sorted_list;
}

// Standard merge sort implementation
void ParallelMergeSorter::merge_sort(int lower, int upper) {
    //determine middle
    int mid = lower + (upper - lower) / 2;

    //if lower is < uppper
    if (lower < upper) {
        //do the left half
        merge_sort(lower, mid);
        //do right side
        merge_sort(mid + 1, upper);
        //bring them all together
        merge(lower, mid, upper);
    }
}


void ParallelMergeSorter::merge(int lower, int middle, int upper) {
    vector<student> temporaryList(upper - lower + 1);

    //for the sublist that needs to be sorted, determine indices
    int i = lower, j = middle + 1, k = 0;

    while (i <= middle && j <= upper) {
      //sort by the grade and if the grades are ties, sort by student id
        if (sorted_list[i].grade < sorted_list[j].grade || (sorted_list[i].grade == sorted_list[j].grade && sorted_list[i].id < sorted_list[j].id)) {
            temporaryList[k++] = sorted_list[i++];
        } else {
            temporaryList[k++] = sorted_list[j++];
        }
    }


//copy remaining left elements to temp list
    while (i <= middle) {
        temporaryList[k++] = sorted_list[i++];
    }

//copy remaining right elements to temp list
    while (j <= upper) {
        temporaryList[k++] = sorted_list[j++];
    }

    for (int idx = 0; idx < k; idx++) {
        sorted_list[lower + idx] = temporaryList[idx];
    }
}

void ParallelMergeSorter::merge_sorted_sublists(const vector<student>& list, int start, int end, vector<student>& merged_list) {
    vector<student> temp_list;
    temp_list.reserve(end - start);

    int mid = (start + end) / 2;
    int i = start, j = mid;

    while (i < mid && j < end) {
        if (list[i].grade < list[j].grade || (list[i].grade == list[j].grade && list[i].id < list[j].id)) {
            temp_list.push_back(list[i]);
            ++i;
        } else {
            temp_list.push_back(list[j]);
            ++j;
        }
    }

    while (i < mid) {
        temp_list.push_back(list[i]);
        ++i;
    }

    while (j < end) {
        temp_list.push_back(list[j]);
        ++j;
    }

    // Copy the merged sublist to the merged_list
    copy(temp_list.begin(), temp_list.end(), back_inserter(merged_list));
}


void ParallelMergeSorter::merge_threads() {
    // Calculate the number of students per thread
    int students_per_thread = sorted_list.size() / num_threads;
    int remaining_students = sorted_list.size() % num_threads;

    // Initialize start and end indices for merging
    int start = 0;

    // Vector to store the merged sorted list
    vector<student> merged_sorted_list;

    // Merge sorted sublists together
    for (int i = 0; i < num_threads; ++i) {
        int end = start + students_per_thread;
        if (i < remaining_students) {
          // Distribute remaining students evenly among threads
            end++; 
        }

        // Merge the sublist into the merged sorted list
        merge_sorted_sublists(sorted_list, start, end, merged_sorted_list);

        start = end;
    }

    // Update the sorted_list with the merged sorted list
    sorted_list.swap(merged_sorted_list);

    // Sort the final merged list
    merge_sort(0, sorted_list.size() - 1);
}


void *ParallelMergeSorter::thread_init(void *args) {
    MergeSortArgs *sort_args = (MergeSortArgs *)args;
    int thread_index = sort_args->thread_index;
    ParallelMergeSorter *ctx = sort_args->ctx;
    int work_per_thread = ctx->sorted_list.size() / ctx->num_threads;
    int lower = thread_index * work_per_thread;
    int upper = (thread_index == ctx->num_threads - 1) ? ctx->sorted_list.size() - 1 : (thread_index + 1) * work_per_thread - 1;


    ctx->merge_sort(lower, upper);
    
    return NULL;
}
