# Example
```bash
gcc coroutine.c example.c -o example && ./example
```
```
1: Hello 1
1: Hello 2
2: Hello 3
2: Hello 3
1: Hello 1
2: Hello 4
```

# Snakes and Ladders
```bash
gcc coroutine.c snl.c -o snl && ./snl
```

```c
int final_destination = 100;

int ladder_foot[] = {10, 30, 50, 70};
int ladder_head[] = {20, 40, 60, 80};

int snake_mouth[] = {15, 35, 55, 75};
int snake_tail[]  = {05, 25, 45, 65};
```


```
Player 0 at position 0 threw 4.
The new location is 4.
Player 1 at position 0 threw 5.
The new location is 5.
Player 2 at position 0 threw 6.
The new location is 6.
Player 3 at position 0 threw 1.
The new location is 1.
Player 0 at position 4 threw 3.
The new location is 7.
Player 1 at position 5 threw 5.
The new location is 10.
But, there's a ladder at 10.
The final position for player 1 is 20.
Player 2 at position 6 threw 3.
The new location is 9.
Player 3 at position 1 threw 4.
The new location is 5.
Player 0 at position 7 threw 1.
The new location is 8.
Player 1 at position 20 threw 6.
The new location is 26.
Player 2 at position 9 threw 4.
The new location is 13.
Player 3 at position 5 threw 6.
The new location is 11.
Player 0 at position 8 threw 1.
The new location is 9.
Player 1 at position 26 threw 1.
The new location is 27.
Player 2 at position 13 threw 4.
The new location is 17.
Player 3 at position 11 threw 3.
The new location is 14.
Player 0 at position 9 threw 1.
The new location is 10.
But, there's a ladder at 10.
The final position for player 0 is 20.
Player 1 at position 27 threw 2.
The new location is 29.
Player 2 at position 17 threw 1.
The new location is 18.
Player 3 at position 14 threw 5.
The new location is 19.
Player 0 at position 20 threw 3.
The new location is 23.
Player 1 at position 29 threw 1.
The new location is 30.
But, there's a ladder at 30.
The final position for player 1 is 40.
Player 2 at position 18 threw 4.
The new location is 22.
Player 3 at position 19 threw 3.
The new location is 22.
Player 0 at position 23 threw 5.
The new location is 28.
Player 1 at position 40 threw 4.
The new location is 44.
Player 2 at position 22 threw 6.
The new location is 28.
Player 3 at position 22 threw 6.
The new location is 28.
Player 0 at position 28 threw 6.
The new location is 34.
Player 1 at position 44 threw 3.
The new location is 47.
Player 2 at position 28 threw 2.
The new location is 30.
But, there's a ladder at 30.
The final position for player 2 is 40.
Player 3 at position 28 threw 2.
The new location is 30.
But, there's a ladder at 30.
The final position for player 3 is 40.
Player 0 at position 34 threw 6.
The new location is 40.
Player 1 at position 47 threw 5.
The new location is 52.
Player 2 at position 40 threw 4.
The new location is 44.
Player 3 at position 40 threw 6.
The new location is 46.
Player 0 at position 40 threw 6.
The new location is 46.
Player 1 at position 52 threw 3.
The new location is 55.
But, there's a snake at 55.
The final position for player 1 is 45.
Player 2 at position 44 threw 1.
The new location is 45.
Player 3 at position 46 threw 5.
The new location is 51.
Player 0 at position 46 threw 2.
The new location is 48.
Player 1 at position 45 threw 3.
The new location is 48.
Player 2 at position 45 threw 2.
The new location is 47.
Player 3 at position 51 threw 4.
The new location is 55.
But, there's a snake at 55.
The final position for player 3 is 45.
Player 0 at position 48 threw 2.
The new location is 50.
But, there's a ladder at 50.
The final position for player 0 is 60.
Player 1 at position 48 threw 2.
The new location is 50.
But, there's a ladder at 50.
The final position for player 1 is 60.
Player 2 at position 47 threw 5.
The new location is 52.
Player 3 at position 45 threw 4.
The new location is 49.
Player 0 at position 60 threw 6.
The new location is 66.
Player 1 at position 60 threw 6.
The new location is 66.
Player 2 at position 52 threw 1.
The new location is 53.
Player 3 at position 49 threw 4.
The new location is 53.
Player 0 at position 66 threw 4.
The new location is 70.
But, there's a ladder at 70.
The final position for player 0 is 80.
Player 1 at position 66 threw 6.
The new location is 72.
Player 2 at position 53 threw 4.
The new location is 57.
Player 3 at position 53 threw 3.
The new location is 56.
Player 0 at position 80 threw 3.
The new location is 83.
Player 1 at position 72 threw 6.
The new location is 78.
Player 2 at position 57 threw 5.
The new location is 62.
Player 3 at position 56 threw 6.
The new location is 62.
Player 0 at position 83 threw 3.
The new location is 86.
Player 1 at position 78 threw 4.
The new location is 82.
Player 2 at position 62 threw 4.
The new location is 66.
Player 3 at position 62 threw 6.
The new location is 68.
Player 0 at position 86 threw 4.
The new location is 90.
Player 1 at position 82 threw 6.
The new location is 88.
Player 2 at position 66 threw 3.
The new location is 69.
Player 3 at position 68 threw 2.
The new location is 70.
But, there's a ladder at 70.
The final position for player 3 is 80.
Player 0 at position 90 threw 1.
The new location is 91.
Player 1 at position 88 threw 3.
The new location is 91.
Player 2 at position 69 threw 1.
The new location is 70.
But, there's a ladder at 70.
The final position for player 2 is 80.
Player 3 at position 80 threw 1.
The new location is 81.
Player 0 at position 91 threw 3.
The new location is 94.
Player 1 at position 91 threw 3.
The new location is 94.
Player 2 at position 80 threw 1.
The new location is 81.
Player 3 at position 81 threw 2.
The new location is 83.
Player 0 at position 94 threw 1.
The new location is 95.
Player 1 at position 94 threw 3.
The new location is 97.
Player 2 at position 81 threw 2.
The new location is 83.
Player 3 at position 83 threw 4.
The new location is 87.
Player 0 at position 95 threw 3.
The new location is 98.
Player 1 at position 97 threw 6.
The new location is 103.
Player 2 at position 83 threw 5.
The new location is 88.
Player 3 at position 87 threw 4.
The new location is 91.
Player 0 at position 98 threw 5.
The new location is 103.
Player 2 at position 88 threw 4.
The new location is 92.
Player 3 at position 91 threw 4.
The new location is 95.
Player 2 at position 92 threw 6.
The new location is 98.
Player 3 at position 95 threw 4.
The new location is 99.
Player 2 at position 98 threw 2.
The new location is 100.
Player 3 at position 99 threw 4.
The new location is 103.
Player 0 at 103
Player 1 at 103
Player 2 at 100
Player 3 at 103
```