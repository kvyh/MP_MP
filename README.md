Coding bits for Matt Parker's Math Puzzles.


# Triangle Solitaire

0  
1 2  
3 4 5  
...

#### Shortest solution examples by side length:

length 4 (5 chains): remove 1; [6-1] [0-3] [8-6-1] [5-0-3-5] [9-2]

length 5 (9 chains): remove 3; [0-3] [6-1] [12-3] [1-6] [9-7] [2-9] [10-12] [13-11] [14-5-3-10-12-3]

beyond length 5 exhaustive search is time-consuming, so these are the shortest I've found.

length 6 (9 chains): remove 12; [5-12] [6-8] [9-7] [15-6] [20-9] [17-8] [3-10] [19-17-15-6] [0-5-12-3-10-12-14-5-3-0]

length 7 (12 chains): remove 10; [21-10] [6-15] [1-6] [23-10-3] [8-6-1] [17-8] [25-12] [2-7-18] [9-2] [0-5] [20-9-2] [27-25-14-12-25-23-21-10-12-5-0-3]
