# MazeGen
MazeGen is a simple maze generator written in AASM which is a simple 16 bit version of ASAM with only a few instructions (learn more [here](https://github.com/Apollointhehouse/AAsm)).

![Maze Example (Seed: 0, Size: 50)](assets/maze.svg)

## Features:
* Generate any maze between 1x1 and 50x50
* Generator takes up less than 8kb
* Mazes can be displayed as SVGs by piping the output into maze_draw
* DFS Maze Generation algorithm

## Args:
  MazeGen takes in two args these being the seed (any number between 0 and 65535) and a size (any number between 1 and 50)

How to Run and How to Build assume maze/ working directory adjust accordingly

## How to Run:
  1. Download the latest version of MazeGen and AASM (these can be found in releases)
  2. Unzip and extract into some directory
  3. Run (assumes maze/ directory program must be ran from outside of the directory): ```java -jar AAsm.jar EXECUTE ~/maze/maze.aaexe | ./maze/maze_draw```

## How to Build:
  1. Download and extract the source code
  2. Build maze_draw (run from maze/): ```cmake -S . -B ../build``` then ```cmake --build ../build```. Now you have a build dir in maze containing maze_draw
  3. Copy maze_draw into maze/ or whatever your main directory is called and delete build if you want
  4. Assemble the maze.aaexe (called from outside the working dir): ```java -jar AAsm.jar ASSEMBLE ~/maze/maze maze.aasm cell_helpers.aasm stack.aasm helpers.aasm shared.aasm temps.aasm constants.aasm maze_gen_mem.aasm```
  5. Run the program (called from outside the working dir): ```java -jar AAsm.jar EXECUTE ~/maze/maze.aaexe | ./maze/maze_draw```
