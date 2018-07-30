# CSCI-1200-Battleship
RPI CSCI-1200 Data Structures, Fall 2015/Spring 2016/Spring 2018, Homework 6 - Battleship Recursion <br />
By Erebuxy and [Huang-1137](https://github.com/Huang-1137)

## Usage
```shell
<filename>
<filename>  find_all_solutions
```
Puzzles with unknown sums and/or unknown ship types are not supported

## Improvements/Optimizations
* Sort the ship list by ship length before recursion.
* Only place the first ship at the upper-left side of the board.
* Keep track of the number of available spaces on the board and stop recursion if there is not enough space.
* Do not rotate a submarine.

## Performance
Complied with `-O3` optimization, run on Intel Core-i7 6700k / Windows subsystem for Linux

Command | # of Solutions | Runtime
--- | :---: | ---: |
`puzzle1.txt find_all_solutions` | 1 | 0.026s
`puzzle2.txt find_all_solutions` | 1 | 0.025s
`puzzle3.txt find_all_solutions` | 1 | 0.026s
`puzzle4.txt find_all_solutions` | 1 | 0.025s
`puzzle4_unknowns.txt` | N/A | N/A
`puzzle5.txt find_all_solutions` | 2 | 0.030s
`puzzle6.txt find_all_solutions` | 24 | 0.037s
`puzzle6_unknowns.txt` | N/A | N/A
`puzzle7.txt find_all_solutions` | 14 | 0.062s
`puzzle8.txt find_all_solutions` | 2 | 0.031s
`puzzle9.txt find_all_solutions` | 152 | 0.150s
`puzzle9_constraints.txt find_all_solutions` | 1 | 0.026s
`puzzle10.txt find_all_solutions` | 3754 | 2.984s
`puzzle10_constraints.txt find_all_solutions` | 1 | 0.029s

## Reference
* http://www.cs.rpi.edu/academics/courses/fall15/csci1200/
* http://www.cs.rpi.edu/academics/courses/spring16/csci1200/
* http://www.cs.rpi.edu/academics/courses/spring18/csci1200
