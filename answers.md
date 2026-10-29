# Answers to the technical test questions

## Part 1 – Large-scale World

> Can you suggest a technical solution to manage a large-scale World without an exponential increase in program memory usage?

With a dense grid, memory grows linearly with the number of units (`width × height`). The goal is therefore to keep the cost per unit as low as possible.

### What is implemented

- **1 byte per unit**: each unit is stored as a `std::uint8_t` (0 = inactive, 1 = active) instead of an `int`, which divides memory by 4.
- **Contiguous storage**: units are stored row by row in a single `std::vector`, so there is no per-row allocation.
- **No allocation to count neighbours**: `World::countActiveNeighbours` returns the number of active neighbours directly instead of building a temporary 
container for each unit.

### How to go further

Memory still grows with the World size (a 1 000 000 × 1 000 000 World would need ~1 TB). To make it depend on the activity instead:

- Split the World into fixed-size blocks (e.g. 64 × 64), stored in a `std::unordered_map` keyed by block position.
- Allocate a block only when it contains active units; a missing block means "all inactive". Empty blocks are freed.
- Each iteration only computes existing blocks and their direct neighbours; border units read from adjacent blocks.
- Inside a block, units are bit-packed (one `std::uint64_t` per row → 512 bytes per block), so dense areas stay compact.

With 1 000 active blocks, the same World takes ~500 KB instead of ~1 TB. 


## Part 2 – Customizable Engines

> Can you describe a way to create various Engines with the ability to customize the rules used for computations?

Currently, the rules are not hard-coded: the birth and survival neighbour counts are passed to `Engine::updateCells` as two vectors, so any rule of this kind can already be used.

To go further, each Engine would receive its rule in its constructor, as a function deciding the next state of a unit:

```cpp
using Rule = std::function<std::uint8_t(std::uint8_t state, int activeNeighbours)>;

Engine dayAndNight([](std::uint8_t state, int n) {
    return (n == 3 || n >= 6 || (state == 1 && n == 4)) ? 1 : 0;
});
Engine gameOfLife([](std::uint8_t state, int n) {
    return (n == 3 || (state == 1 && n == 2)) ? 1 : 0;
});
```

The Engine iterates over the World and counts the neighbours, and the rule only decides the new state. Any rule can be expressed, not only lists of neighbour counts.


## Part 4 – Pseudo infinite grid

> How can you modify your implementation to simulate a pseudo infinite grid?

Currently, positions beyond the grid are ignored in `World::countActiveNeighbours`, so the borders act as walls of inactive units and patterns die when they reach them.

### Wrap-around (torus)

The simplest change is to connect opposite borders: the right neighbour of the last column is the first column, and the bottom neighbour of the last row is the first row. Instead of skipping out-of-range positions, they are wrapped with a modulo:

```cpp
const int neighbourRow    = (row + dy + _height) % _height;
const int neighbourColumn = (column + dx + _width) % _width;
```

Only `countActiveNeighbours` changes; the Engine and the Rendering stay the same, and memory usage does not change. A pattern leaving on one side comes back on the other, so the World has no edge. 

### Growing World 

To have no edge at all, the World can grow with the activity, using the block storage described in Part 1:

- Coordinates become signed and unbounded (`std::int64_t`), and blocks are created on demand when an active unit reaches the border of an existing block.
- Only blocks containing active units (and their neighbours) are stored and computed, so memory depends on the activity, not on the covered area.
- The Rendering shows a fixed-size viewport (e.g. the terminal size) that can follow the activity or be moved by the user.



## Part 4 – Improvements

> Which features would you add to improve the program?

**Usability**
- Command-line options for width, height, ratio, number of iterations, delay between frames, seed and rule , instead of constants in `main`.

- Display statistics: iteration number, number of active units, time per iteration.


**Code quality**
- Unit tests for the World (neighbour counting on borders/corners) and the Engine (known patterns such as stable blocks or oscillators).
- Abstract `Rendering` interface to support other outputs (colors, file export, GUI) without changing the World or the Engine.
- Rule stored in the Engine (see Part 2) so `main` no longer passes the rules at each iteration.

