#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "p1_process.h"
#include "p1_threads.h"


using namespace std;


double student_list_median(vector<student> students) {
    int list_size = students.size();
    double median = 0.0;

    // If the list size is even
    if (list_size % 2 == 0) {
        median = (students[list_size / 2 - 1].grade + students[list_size / 2].grade) / 2.0;
    } else { // If the list size is odd
        median = students[list_size / 2].grade;
    }

    return median;
}


double student_list_standard_dev(vector<student>students, double mean){
int list_size = students.size();
double sum_of_squared_deviations = 0.0;


//get deviation value for each number
   for (int i = 0; i < list_size; ++i) {
       double deviation = students[i].grade - mean;
       sum_of_squared_deviations += deviation * deviation;
   }
   // Calculate the standard deviation
   double standard_deviation = sqrt(sum_of_squared_deviations / list_size);
   return standard_deviation;

}


double student_list_mean(vector<student>students){
 double sum= 0 ;
 for(int i =0 ; i< students.size();i++){
   sum+=students[i].grade;
 }
return sum / students.size();
}

// This function should be called in each child process right after forking
// The input vector should be a subset of the original files vector
void process_classes(vector<string> classes, int num_threads) {
 printf("Child process is created. (pid: %d)\n", getpid());
 // Each process should use the sort function which you have defined    
 // in the p1_threads.cpp for multithread sorting of the data.

 for (int i = 0; i < classes.size(); i++) {
   // get all the input/output file names here
   string class_name = classes[i];
   char buffer[40];
   sprintf(buffer, "input/%s.csv", class_name.c_str());
   string input_file_name(buffer);

   sprintf(buffer, "output/%s_sorted.csv", class_name.c_str());
   string output_sorted_file_name(buffer);

   sprintf(buffer, "output/%s_stats.csv", class_name.c_str());
   string output_stats_file_name(buffer);

   vector<student> students;
   double sum_of_grades = 0;
   int num_of_students = 0;
   // Your implementation goes here, you will need to implement:
   // File I/O
   //  - This means reading the input file, and creating a list of students,
   //  see p1_process.h for the definition of the student struct
   //
   ifstream read_input_file(input_file_name.c_str());

   if(!read_input_file){
     cerr << "Error: cannot open input file: "<< input_file_name << endl;
     continue;
   }
    //skip the first line for each file
   string first_line;
   getline(read_input_file,first_line);

  string line;
   //process the rest of the file
 
  int r=0;
  while (getline(read_input_file, line)){
      //r++;
      //getline(read_input_file, line);

       // Tokenize the line by comma
       stringstream ss(line);
       string token;
       vector<string> tokens;

       // Get the 2 values from the string
       while (getline(ss, token, ',')) {
           tokens.push_back(token);
       }

       if (tokens.size() < 2) {
           cout << "Did not get values" << endl;
           continue; // Skip processing this line
       }

       // Parse student ID as unsigned long
       char* endptr; // To check if conversion was successful
       unsigned long student_id = strtoul(tokens[0].c_str(), &endptr, 10);
       if (*endptr != '\0') {
           cerr << "Error: Invalid student ID." << endl;
           continue; // Skip processing this line
       }

       // Parse grade as double
       double grade = strtod(tokens[1].c_str(), &endptr);
       if (*endptr != '\0') {
           cerr << "Error: Invalid grade." << endl;
           continue; // Skip processing this line
       }

       // Create student object and store in vector
       students.push_back(student(student_id, grade));
   }


   read_input_file.close();

   // Run multi threaded sort
   ParallelMergeSorter sorter(students, num_threads);
   vector<student> sorted = sorter.run_sort();
 
   FILE *output_file_sorted;
   output_file_sorted = fopen(output_sorted_file_name.c_str(),"w");

   if(output_file_sorted == NULL){
     cerr <<"Cannot open output file" << endl;
   }
   fprintf(output_file_sorted, "Rank,Student ID,Grade\n");

//reverse the sorting from the sorter since i did it in descending order
vector<student> sorted_ascending;
for (int t = sorted.size() - 1; t >= 0; t--) {
    sorted_ascending.push_back(sorted[t]);
}

      for(int i=0 ;i < sorted_ascending.size();i++){
   fprintf(output_file_sorted,"%d,%lu,%.10f\n",i+1,sorted_ascending[i].id,sorted_ascending[i].grade);
  }


  // for(int i=0 ;i < sorted.size();i++){
  // fprintf(output_file_sorted,"%d,%lu,%.10f\n",i+1,sorted[i].id,sorted[i].grade);
  //}



  fclose(output_file_sorted);
//calculate the median
   double mean = student_list_mean(sorted);
   double median = student_list_median(sorted);
   double st_dev = student_list_standard_dev(sorted,mean);

   FILE *stats_file;
   stats_file = fopen(output_stats_file_name.c_str(),"w");
   fprintf(stats_file, "Average,Median,Std. Dev\n");
   fprintf(stats_file,"%.3f,%.3f,%.3f\n",mean,median,st_dev);

 }
 // child process done, exit the program
 printf("Child process is terminated. (pid: %d)\n", getpid());
 exit(0);
}


void create_processes_and_sort(vector<string> class_names, int num_processes, int num_threads) {
  vector<pid_t> child_pids;
    int classes_per_process = max(class_names.size() / num_processes, 1ul);

    for (int i = 0; i < num_processes; ++i) {
        size_t start_idx = i * classes_per_process;
        size_t end_idx = (i == num_processes - 1) ? class_names.size() : (i + 1) * classes_per_process;

        pid_t pid = fork();
        if (pid == 0) {  // Child process
            vector<string> sublist(class_names.begin() + start_idx, class_names.begin() + end_idx);
            process_classes(sublist, num_threads);
            exit(0);
        } else if (pid > 0) { // Parent process
            child_pids.push_back(pid);
        } else { // Error
            cerr << "Failed to fork" << endl;
            exit(1);
        }
    }

    // Wait for all child processes to finish
    for (size_t i = 0; i < child_pids.size(); ++i) {
        waitpid(child_pids[i], NULL, 0);
    }
}
