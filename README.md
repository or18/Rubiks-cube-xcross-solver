# Rubiks-cube-xcross-solver
C++ source to solve Rubik's cube cross, xcross, xxcross, xxxcross, xxxxcross, last layer substeps (OLL, ZBLL, etc.), last layer, 
# How to use
## sample script
```c++
    solve_F2L( //sample program to output xxcross solutions
        "R' F D L2 U' R2 U' F2 U' F2 R2 B2 D2 F L' R' D2 F' D L R", //scramble
        "z2", //pre rotation
        false, //solve BL slot or not
        false, //solve BR slot or not
        false, //solve FR slot or not
        false, //solve FL slot or not
        12, //max solution length
        100, //max solution number
        "UDLRFB" //face restrict (use U, D, L, R, F, B)
        );
```
## compile and run
```sh
!g++ -Ofast -mtune=native -march=native test.cpp -pthread
!./a.out
```

# Performance
## Compiled and executed on Google colab by the command
```sh
!apt-get install time (to use /usr/bin/time command)
!g++ -Ofast -mtune=native -march=native main.cpp -pthread
!/usr/bin/time -v ./a.out
```
## cross
### script in main()
```c++
    solve_F2L( //sample program to output xxcross solutions
        "R' F D L2 U' R2 U' F2 U' F2 R2 B2 D2 F L' R' D2 F' D L R", //scramble
        "z2", //pre rotation
        false, //solve BL slot or not
        false, //solve BR slot or not
        false, //solve FR slot or not
        false, //solve FL slot or not
        7, //max solution length
        100, //max solution number
        "UDLRFB" //face restrict (use U, D, L, R, F, B)
        );
```
### result
```txt
	Command being timed: "./a.out"
	User time (seconds): 0.02
	System time (seconds): 0.00
	Percent of CPU this job got: 96%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.03
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 4204
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 0
	Minor (reclaiming a frame) page faults: 431
	Voluntary context switches: 1
	Involuntary context switches: 74
	Swaps: 0
	File system inputs: 0
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
```
## xcross
### script in main()
```c++
    solve_F2L( //sample program to output xxcross solutions
        "R' F D L2 U' R2 U' F2 U' F2 R2 B2 D2 F L' R' D2 F' D L R", //scramble
        "z2", //pre rotation
        true, //solve BL slot or not
        false, //solve BR slot or not
        false, //solve FR slot or not
        false, //solve FL slot or not
        10, //max solution length
        100, //max solution number
        "UDLRFB" //face restrict (use U, D, L, R, F, B)
        );
```
### result
```txt
	Command being timed: "./a.out"
	User time (seconds): 9.01
	System time (seconds): 0.83
	Percent of CPU this job got: 98%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:09.96
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 573648
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 0
	Minor (reclaiming a frame) page faults: 142713
	Voluntary context switches: 2
	Involuntary context switches: 581
	Swaps: 0
	File system inputs: 0
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
```
## xxcross
### script in main()
```c++
    solve_F2L( //sample program to output xxcross solutions
        "R' F D L2 U' R2 U' F2 U' F2 R2 B2 D2 F L' R' D2 F' D L R", //scramble
        "z2", //pre rotation
        true, //solve BL slot or not
        false, //solve BR slot or not
        false, //solve FR slot or not
        true, //solve FL slot or not
        12, //max solution length
        100, //max solution number
        "UDLRFB" //face restrict (use U, D, L, R, F, B)
        );
```
### result
```txt
	Command being timed: "./a.out"
	User time (seconds): 22.07
	System time (seconds): 0.99
	Percent of CPU this job got: 165%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:13.93
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 858632
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 0
	Minor (reclaiming a frame) page faults: 213999
	Voluntary context switches: 20
	Involuntary context switches: 3623
	Swaps: 0
	File system inputs: 0
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
```
## xxxcross
### script in main()
```c++
    solve_F2L( //sample program to output xxcross solutions
        "R' F D L2 U' R2 U' F2 U' F2 R2 B2 D2 F L' R' D2 F' D L R", //scramble
        "z2", //pre rotation
        true, //solve BL slot or not
        true, //solve BR slot or not
        false, //solve FR slot or not
        true, //solve FL slot or not
        14, //max solution length
        100, //max solution number
        "UDLRFB" //face restrict (use U, D, L, R, F, B)
        );
```
### result
```txt
	Command being timed: "./a.out"
	User time (seconds): 34.82
	System time (seconds): 0.89
	Percent of CPU this job got: 168%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:21.19
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 1143780
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 0
	Minor (reclaiming a frame) page faults: 285281
	Voluntary context switches: 27
	Involuntary context switches: 6848
	Swaps: 0
	File system inputs: 0
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
```
## xxxxcross
### script in main()
```c++
    solve_F2L( //sample program to output xxcross solutions
        "R' F D L2 U' R2 U' F2 U' F2 R2 B2 D2 F L' R' D2 F' D L R", //scramble
        "z2", //pre rotation
        true, //solve BL slot or not
        true, //solve BR slot or not
        true, //solve FR slot or not
        true, //solve FL slot or not
        16, //max solution length
        100, //max solution number
        "UDLRFB" //face restrict (use U, D, L, R, F, B)
        );
```
### result
```txt
	Command being timed: "./a.out"
	User time (seconds): 153.01
	System time (seconds): 1.89
	Percent of CPU this job got: 113%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 2:16.80
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 1428868
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 0
	Minor (reclaiming a frame) page faults: 356568
	Voluntary context switches: 47
	Involuntary context switches: 18877
	Swaps: 0
	File system inputs: 0
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
```
