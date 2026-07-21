# Chess Engine

## Overview

This project was developed as part of a systems programming course. The assignment challenged students to design and implement a fully autonomous chess engine in C that could compete against other student-created engines in a tournament environment.

Unlike a traditional chess application with a graphical interface, this project focused on algorithm design, game-state management, and communication through a predefined engine protocol.

## Project Goals

### The primary objectives of this project were:

- Implement a legal move generator for all chess pieces
- Maintain and update board state efficiently
- Develop decision-making logic for move selection
- Compete against other engines under tournament time constraints
- Produce an engine capable of autonomous play without external libraries or network communication
- Technical Constraints

### To emphasize systems programming concepts, the engine was developed under several restrictions:

- Implemented entirely in C
- Limited to the standard C library
- No external chess libraries or engines
- No file creation or network communication
- Required to communicate moves through a custom tournament protocol

These constraints encouraged efficient data structures, careful memory management, and algorithmic problem solving. The assignment specifications required strict adherence to a judge-based tournament system where engines competed in timed matches.

## Features

### Depending on what you implemented, you could include:

- Legal move generation
- Check and checkmate detection
- Piece capture evaluation
- Position scoring heuristics
- Search algorithms (Minimax, Alpha-Beta pruning, etc.)
- Time-aware move selection
- Tournament compatibility
- Educational Value

### This project served as an introduction to several important computer engineering concepts:

- Complex state-machine design
- Recursive search algorithms
- Performance optimization in C
- Memory-safe systems programming
- Competitive AI development
- Competition Environment

Submitted engines were evaluated in a tournament setting, competing against other student engines and benchmark opponents. Performance depended on both strategic strength and adherence to protocol requirements.
