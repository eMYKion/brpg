# [UNDER CONSTRUCTION] Behavior-Driven Characters RPG (BRPG)

## "Requirements"
1. Ubuntu 16.04
2. development to X11 windowing system via `sudo apt install libx11-dev`
3. valgrind via `sudo apt install valgrind`

## Instructions
1. `make clean`
2. `make`
3. `./bin/run`

## Debugging

### Memory Leak Checking

```valgrind --leak-check=yes ./bin/run```
