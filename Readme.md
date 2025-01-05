# Author
Eli Davidson

# contact 
elid@uci.edu

# Description

● p1_exec receives two arguments: 1) the specified number of child processes and 2) the number of threads per child process.


● Your program needs to create multiple processes to handle multiple files in parallel. Each child
process handles at least one file. In other words, one single child process may handle multiple files.


● For each input file, you will need to first sort the input data read from a file with a multithreaded
sorting algorithm (merge sort is suggested, see https://en.wikipedia.org/wiki/Merge_sort)


● Then, calculate the following statistics: Average, Median, and Standard Deviation.


● Only the child process should create multiple threads to perform the sorting. So, the process
hierarchy will be the following: Main process (spawns) -> Child processes (spawns) -> Threads.


● You must not waste any resources; thus, you should not create any non-working processes or
threads.


● The results need to be saved to the output directory with the following filename: <input filename> stats.csv and <input filename>_sorted.csv (i.e., os_stats.csv and os_sorted.csv).


■ NOTE: The separator here is an underscore (_), not a hyphen


● When a process is created and terminated, you should print a message (see below) with its process
id to indicate the process has been created/terminated (you don’t need to do this for threads).


● The main process must complete its behavior after its child processes.
The complete program will provide the following output. Pay attention to the order in which processes
terminate. (The main process terminates last)
./p1_exec 2 4<br>

Main process is created. (pid: xxxx)<br>
Child process is created. (pid: yyyy)<br>
Child process is created. (pid: zzzz)<br>
...
Child process is terminated. (pid: yyyy)<br>
Child process is terminated. (pid: zzzz) <br>
Main process is terminated. (pid: xxxx)<br>


The complete program will provide the following output files:
os-stats.csv content: (please display at least 3 decimal places)<br>

Average,Median,Std. Dev <br>
XX.XXX,XX.XXX,XX.XXX


os-sorted.csv content: (Notice that rank starts at 1, and don’t worry about the formatting for the # of
digits to display after the decimal)


Rank,Student ID,Grade <br>
1,XXXXXXXXXX,XX.XXX <br>
2,XXXXXXXXXX,XX.XXX <br>
3,XXXXXXXXXX,XX.XXX <br>
....




# score
100%
