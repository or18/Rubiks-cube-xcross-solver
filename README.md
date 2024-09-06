# Rubiks-cube-xcross-solver
C++ source to solve Rubik's cube cross, xcross, xxcross, xxxcross, xxxxcross, last layer substeps (OLL, ZBLL, etc.), last layer. Demo solver: https://or18.github.io/Rubiks-cube-xcross-solver/

# Note
- Memory required (for main.cpp, cli.cpp). For more detail performance, please see Performance.
  
| solver  | memory |
| --- | --- |
| cross  | 4.2 MB  |
| xcross  | 574 MB  |
| xxcross  | 859 MB  |
| xxxcross  | 1.15 GB  |
| xxxxcross  | 1.43 GB  |
| LL substeps  | 1.43 GB  |
| LL  | 1.43 GB  |
| LL AUF  | 1.43 GB  |

- Add the following three lines to further speed up the process
```c++
#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
```
- CLI is available. See cli.cpp and cli_output.txt.
- Lite version is available. See cli_lite.cpp and cli_lite_without_thread.cpp. For cross, xcross, xxcross and xxxcross, this cli is better.
- Demo solver is available. See https://or18.github.io/Rubiks-cube-xcross-solver/
- cli.cpp and cli_lite.cpp testing on github codespace. <br>
[cli.cpp](https://youtu.be/76oK2FGWia8?si=AcBwhUqRPq9PagZN) <br>
[cli_lite.cpp](https://youtu.be/htnWnpDn01Y?si=O1tiVga04Jede1qP) <br>
# How to use
## sample script
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
## compile and run
```sh
!g++ -Ofast -mtune=native -march=native main.cpp -pthread
!./a.out
```
## output
```txt
1: z2 R D L2 F' R2 D2 L' U L2 B2 
2: z2 U D' F' L' R' U D2 R2 F2 U2 B
3: z2 U2 F' R2 D' B D2 R' U2 F L' D'
4: z2 U' L' D2 R U D L2 D2 B' U' L'
5: z2 U' L' F2 D2 R D' B' L' B' U' B
6: z2 D L2 D' L2 R U2 F U' L' D' B
7: z2 D F' R B U B2 L D' L' U L'
8: z2 D2 R' B2 U' R D' B' U' R2 F L'
9: z2 D2 F' L' D' F L' D' R F2 L' D'
10: z2 D' R' U' F' D2 B2 R' F U L2 D2
11: z2 D' F' U L R' B2 R2 U2 F' D' L2
12: z2 D' F' U L' R' D2 F' U R2 F2 B2
13: z2 D' F' L' R' D2 F' U R2 F2 B2 R
14: z2 L2 R U2 F U L U2 D L' D2 B
15: z2 L2 R U2 F D F2 U' L' D2 R2 B
16: z2 L2 R U2 F D F2 U' L' D2 B R2
17: z2 L2 R U2 F L U' D F' L' D2 B
18: z2 L2 R U2 F B2 L' U' D L D2 B
19: z2 L2 R B R D' R' U L' F D' B
20: z2 L2 F' U D2 R D' R' B' L' U B2
21: z2 L' R D F R2 B2 U L' R2 D' R'
22: z2 L' F R B2 L2 U' F L D' R B2
23: z2 L' F2 D B2 U' L' B R' D B' R
24: z2 L' F2 B2 L D' R F' U' F2 L B
25: z2 L' F2 B' D' B' U' R F L U' B
26: z2 L' F' U F2 R U F D F L' D2
27: z2 L' B2 U L R' B' L2 U F2 D B
28: z2 R U L2 U' F' L' F2 D F L' D2 
29: z2 R U B2 U' L2 U2 F U' L' D' B
30: z2 R U' D' L2 F2 L2 R B D F2 B2
31: z2 R U' L2 U2 B2 L2 F U' L D' B
32: z2 R U' F D' R U' B' L' R U2 L
33: z2 R D L2 D' L2 U2 F U' L' D' B
34: z2 R D L2 F' D R' D2 L2 F2 L' D'
35: z2 R D L2 F' R2 D2 L' U L2 B2 U
36: z2 R D L2 F' R2 D2 L' U L2 B2 U2
37: z2 R D L2 F' R2 D2 L' U L2 B2 U'
38: z2 R F' D L' D2 F2 L2 B' D' F2 R'
39: z2 R F' D' L' F2 L2 R B' D' F2 R'
40: z2 R F' D' L' F2 L2 B' R D' F2 R'
41: z2 R B' L2 U D' L' F D' B U' F'
42: z2 R2 D R' U B2 L U' F U L2 D'
43: z2 R' U F2 D L' B2 U2 D2 B D B2
44: z2 R' D R2 U L' F B2 D F' D2 B'
45: z2 B2 L' F B2 U' D R U' B' L' B'
46: z2 B' L' R F R U D F2 L' D2 F' 
47: z2 U D L2 D2 F' L' R' U' L2 B2 D2 B
48: z2 U D L' F D2 F' R' D' R2 B2 D' B'
49: z2 U D L' B R2 B' R' D' R2 B2 D' B'
50: z2 U D F D2 F' L' R' U' B2 L2 D2 B'
51: z2 U D F' R' U2 R' U' B' R2 F2 L' B'
52: z2 U D F' R' U2 R' B' U' R2 F2 L' B'
53: z2 U D B R2 B' L' R' D' R2 B2 D' B'
54: z2 U D2 R' U D' R2 F2 U' B' L' U B
55: z2 U D2 F' L' D2 R U F' U' B2 D' B'
56: z2 U D' L2 F2 R' F' L' B2 R2 D' L2 B
57: z2 U D' R F R U2 R U' L B' R2 F
58: z2 U D' R2 B2 R' F L' D2 B' D L2 B2
59: z2 U D' R' F U R' B2 D L R' F2 B
60: z2 U D' F R' U R' B2 D L R' F2 B
61: z2 U D' F B2 D2 L' R B2 U B2 D B
62: z2 U D' F' D2 L' R B2 U' D2 B2 D B
63: z2 U D' F' L R' B2 R2 U2 D F' D2 L2
64: z2 U D' F' L R' B2 R2 U2 D' L2 U2 B
65: z2 U D' F' L' R' U D2 R2 F2 U2 B U
66: z2 U D' F' L' R' U D2 R2 F2 U2 B U2
67: z2 U D' F' L' R' U D2 R2 F2 U2 B U' 
68: z2 U D' F' L' R' U' D2 F2 U2 R2 F2 B
69: z2 U D' F' R' F L' U D2 R2 F2 U2 B
70: z2 U D' F' R' F2 L' U D2 R2 F2 U2 B
71: z2 U D' F' R' F' L' U D2 R2 F2 U2 B
72: z2 U D' B2 R2 F D2 L' R D B2 L2 B'
73: z2 U L' U F' R' B2 U B D R' F2 B2
74: z2 U R D' F2 U L2 U R F2 B D B2
75: z2 U R F R2 U2 R' U' R D' R F2 B'
76: z2 U R F' R2 F2 D L' D' R2 B2 D' B'
77: z2 U R F' R2 F2 R2 D L' D' B2 D' B'
78: z2 U R B' L2 U2 F B' U L U2 D' B
79: z2 U R' U' R2 U' F D' R U' F2 B' R
80: z2 U R' B' L' R' F U' D B' R F B2
81: z2 U F U' L F2 D R' B' U F2 U2 L'
82: z2 U F2 U2 R U2 F U D L' D2 B' R2
83: z2 U F2 U2 R U2 F D L' U D2 B' R2
84: z2 U F' R F' D B2 L' F2 U2 D' F2 B
85: z2 U B2 L B2 L2 D2 B2 U' D' B' U' L'
86: z2 U2 D' R' U' F L F2 R2 U2 F2 B' R
87: z2 U2 D' F' U' L' R' U D2 R2 F2 U2 B 
88: z2 U2 L' U D2 R D L2 D2 B' U' L' R2
89: z2 U2 L' U D' L2 D' R D' B' U' L' R2
90: z2 U2 L' U' D' L2 F L R' U R2 F2 B
91: z2 U2 L' F2 D2 R U D' B' U2 L' B R
92: z2 U2 L' F2 L' R F U' D L' D2 R B
93: z2 U2 L' F2 L' R2 D R' F U' L' D' B
94: z2 U2 L' F' U D R' U' B' R2 F L' B'
95: z2 U2 L' B D' F U' L R' U R2 F' B2
96: z2 U2 R F D' F U' F' U2 F2 R F2 B'
97: z2 U2 R' U2 D R' B U' F2 B U2 L' B
98: z2 U2 R' U' D L' D B2 D B D B2 R'
99: z2 U2 F2 U R U' F D L U2 L2 D2 B'
100: z2 U2 F2 R2 D R D2 L2 B' U R D F2
```

# Performance
## Test Environment
11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz <br>
Cores: 4 <br>
Processors: 8 <br>
RAM: 16 GB <br>

## F2L solver
scramble: B U2 B' R F B' D R B' R2 B2 L D2 B2 R' U2 R' U2 F2 L2 <br>
rotation: z2 <br>
solution count: 100 <br>
face restrict: UDLFRF <br>

| slot option | setup (ms) | search (ms) | Memory (MB) | 
| --- | --- | --- | --- |
| None | 11  | 25  | 8  |
| BL | 4784  | 31  | 564  |
| BL FL | 5786 | 41 | 842 |
| BL BR FL | 5611 | 920 | 1121 |
| BL BR FR FL | 5801 | 35133 | 1399 |

lite version

| slot option | setup (ms) | search (ms) | Memory (MB) | 
| --- | --- | --- | --- |
| None | 15  | 31  | 8  |
| BL | 379 | 31  | 42  |
| BL FL | 312 | 94 | 59 |
| BL BR FL | 392 | 5357 | 76 |
| BL BR FR FL | 372 | 329173 | 94 |

## LL substeps solver
scramble1: L F R2 L' U2 F R F R2 F2 U2 R2 F' U2 F U2 F' (last layer) <br>
rotation: <br>
solution count: 50 <br>

| LL option |  face restrict | setup (ms) | search (ms) | Memory (MB) |
| --- | --- | --- | --- | --- |
| EO | UDLRFB | 6622 | 2201 | 1400 |
| EO | UDRF | 6634 | 374 | 1400 |
| EO | URF | 6073 | 31 | 1400 |
| EO CO | UDLRFB | 6441 | 6721 | 1400 |
| EO CO | UDRF | 6278 | 6103 | 1400 |
| EO CO | URF | 6116 | 579 | 1400 |
| EO CO CP | UDLRFB | 6225 | 50698 | 1400 |
| EO CO CP | UDRF | 6608 | 45860 | 1400 |
| EO CO CP | URF | 6486 | 3217 | 1400 |

scramble2: U' L2 F' L' U F' R' U2 D2 F2 R D2 L' U2 F2 R' B2 (last layer & FR slot) <br> 
rotation: <br>
solution count: 50 <br>

| LL option |  face restrict | setup (ms) | search (ms) | Memory (MB) |
| --- | --- | --- | --- | --- |
| EO | UDLRFB | 6554 | 502 | 1400 |
| EO | UDRF | 6317 | 391 | 1400|
| EO | URF | 6061 | 31 | 1400 |
| EO CO | UDLRFB | 6428 | 25144 | 1400 |
| EO CO | UDRF | 6160 | 16189 | 1400 |
| EO CO | URF | 6141 | 1238 | 1400 |
| EO CO CP | UDLRFB | 6106 | 336487 | 1400 |
| EO CO CP | UDRF | 6064 | 192209 | 1400 |
| EO CO CP | URF | 6672 | 13133 | 1400 |

## LL solver
scramble1: L2 R2 U R2 U' B2 D B2 L2 U R2 U L R' F2 L' R' <br>
rotation: <br>
solution count: 20 <br>

| face restrict | setup (ms) | search (ms) | Memory (MB) |
| --- | --- | --- | --- |
| UDLRFB | 6006 | 34934 | 1400 |
| UDRF | 6400 | 39588 | 1400 |
| URF| 6298 | 3256 | 1400 |

scramble2: R D L2 D' R' D L2 U L2 F2 D R2 U F2 D2 L2 B2 <br>
rotation: <br>
solutions count: 20 <br>

| face restrict | setup (ms) | search (ms) | Memory (MB) |
| --- | --- | --- | --- |
| UDLRFB | 6465 | 113948 | 1400 |
| UDRF | 6693 | 57412 | 1400 |
| URF| 6431 | 14209 | 1400 |

scramble3: U2 F2 L2 D2 F2 U2 B2 R' F2 D2 L' B D L' D L' F2 R' <br>
rotation: <br>
solutions count: 20 <br>

| face restrict | setup (ms) | search (ms) | Memory (MB) |
| --- | --- | --- | --- |
| UDLRFB | 6431 | 200947 | 1400 |
| UDRF | 6030 | 422548 | 1400 |
| URF| 6343 | 18259 | 1400 |
