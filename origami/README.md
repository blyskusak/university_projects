# Origami Layers

This project implements a program to study how many paper layers overlap at a given point after a series of folds.  

## Problem Statement

We are given a sequence of paper sheets, each defined as either:
- a rectangle,
- a circle,
- or a folded sheet based on a previously defined sheet.

We also receive queries asking how many layers of paper are present at specific coordinates of a given sheet.

### Input format

1. The first line contains two integers `n` and `q`:
   - `n` — number of sheets,
   - `q` — number of queries.

2. The next `n` lines describe the sheets. Each description has one of the following forms:

- **Rectangle**: P x1 y1 x2 y2:
A closed axis-aligned rectangle with bottom-left corner `(x1, y1)` and top-right corner `(x2, y2)`.  
A pin inside or on the border pierces the sheet once.
- **Circle**: K x y r:
A closed circle centered at `(x, y)` with radius `r`.  
A pin inside or on the border pierces the sheet once.
- **Fold**: Z k x1 y1 x2 y2:
A new sheet obtained by folding sheet `k` along the line passing through `(x1, y1)` and `(x2, y2)`.  
Folding convention: the right-hand side of the line (looking from `(x1,y1)` toward `(x2,y2)`) is folded over onto the left-hand side.  
- Points on the right side after the fold → 0 layers.  
- Points on the line → same number of layers as before folding.  
- Points on the left → layers from the original point + layers from the mirrored point across the fold line.

3. The following `q` lines describe the queries k x y:
meaning: "How many layers are at point `(x,y)` of sheet `k`?"

### Output format
For each query, output one line with a single integer: the number of layers pierced by the pin.
