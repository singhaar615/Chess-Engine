# HW 21: Chess Engine ♟️

**Assignment Points:** 10

**Due Date:** December 13th

**[Submission Link](https://hw21.ece264.com)**

In this assignment, you'll be building a chess engine from scratch. You are permitted to use AI tools for assistance and to help with coding, but all code must be your own.

-----

## Project Requirements

1.  **Language:** The engine must be written entirely in **C**.
2.  **Protocol:** Your engine must strictly adhere to the defined communication protocol.
3.  **Libraries:** Only the C standard library is permitted.
4.  **Restrictions:** The engine may **not** create any files or communicate over a network.

-----

## Tournament Protocol

A judge program will facilitate a chess match between two randomly selected engines. The judge program will:

1.  **Assign Colors:** Randomly assign colors.
      * `1` for **White**
      * `2` for **Black**
2.  **Start the Game:** The engine assigned **White** must make the first move immediately after receiving its color.
3.  **Time Control:** The time control is **5+0** (5 minutes per player, no increment). Time remaining for each player is sent in milliseconds.
4.  **Move Communication:** The judge program reads, validates, and relays each move between the two engines.
5.  **Game End:** The game ends and the judge program terminates both engines when one of the following occurs:
      * A checkmate is delivered.
      * A player's time runs out.
      * An illegal move is made.
      * The position is a draw.

-----

## Reporting Moves

You must populate a `move_t` variable to report your move. Follow the standard algebraic notation for the starting and ending squares. Additionally, the last character in the move_t variable **MUST** be an `x` except if you are promoting a pawn, in which case it will be `q` for queen, `b` for bishop, `n` for knight, `r` for rook (note that it is lowercase).

### Example: Playing 'e2e4'

To play the move 'e2e4', your code should look like this:

```c
move_t my_move;
my_move[0] = 'e';
my_move[1] = '2';
my_move[2] = 'e';
my_move[3] = '4';
my_move[4] = 'x';
```

### Example: Promoting the 'a' pawn to a rook

To promote the 'a' pawn, your code should look like this:

```c
move_t my_move;
my_move[0] = 'a';
my_move[1] = '7';
my_move[2] = 'a';
my_move[3] = '8';
my_move[4] = 'r';
```

After populating the variable, call the provided `send_move(my_move);` function.

**Important:** An **illegal move** will result in an immediate forfeiture of the game.

-----

## Notes on Draw and Time-out Rules

The judge program's rules for draws and time-outs deviate from standard FIDE rules to simplify the assignment and focus on speed and legality.

* **Draw by Repetition & 50-Move Rule:** Your engine is **not** required to implement draws based on the 50-move rule or three-fold repetition. Games will continue until a checkmate or a time-out occurs.
* **Time-out Rule:** If a player runs out of time, they **immediately lose the game**, regardless of the board state. This overrides standard FIDE rules where a time-out can result in a draw if the opponent lacks sufficient material to win. For this assignment, a player with insufficient material can still win if their opponent's time expires.
* **Draw by Agreement:** Engines are **not** permitted to offer or accept a draw.

***

## Submitting Your Engine 🚀

1.  **User ID:** Place your user ID (found on Brightspace under "Grades") into a file named **`name.txt`**. The file should contain only your user ID and nothing else.

2.  **Compile & Clean:**
    * Before submitting, run `make submit` to verify that your code compiles correctly.
    * Next, run `make clean` to remove all generated object files (`.o`) and executables.

3.  **Submit:**
    * Go to the provided submission link.
    * Select **all** files within your chess engine directory (ensure that you haven't modified the files you shouldn't).
    * Submit your files. You should receive a confirmation alert upon successful submission.

4.  **Evaluation & Ranking:**
    * The backend will begin evaluating your engine against others. During this time, you will be unable to resubmit.
    * To view your ranking on the leaderboard, go to the leaderboard page and look for the **first 16 characters** of your user ID (it may take sometime for the leaderboard to update your ranking after a submission).
    * After the due date, all submitted chess engines (we store only your latest submission) will play against a 250 ELO Stockfish engine. If your submission beats Stockfish, you will earn the 10 points; otherwise, you will earn 8 points. If the submission is invalid (using external libraries), you will not earn any points. We may award bonus points to the highest ranked submissions on the leaderboard.

***

## Additional Updates

All further clarifications and updates regarding this assignment will be posted on **Piazza**.
