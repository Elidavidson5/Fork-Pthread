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
./p1
exec 2 4
_
Main process is created. (pid: xxxx)
Child process is created. (pid: yyyy)
Child process is created. (pid: zzzz)
...
Child process is terminated. (pid: yyyy)
Child process is terminated. (pid: zzzz)
Main process is terminated. (pid: xxxx)


The complete program will provide the following output files:
os-stats.csv content: (please display at least 3 decimal places)

Average,Median,Std. Dev
XX.XXX,XX.XXX,XX.XXX


os-sorted.csv content: (Notice that rank starts at 1, and don’t worry about the formatting for the # of
digits to display after the decimal)


Rank,Student ID,Grade
1,XXXXXXXXXX,XX.XXX
2,XXXXXXXXXX,XX.XXX
3,XXXXXXXXXX,XX.XXX
....




# score
100%
