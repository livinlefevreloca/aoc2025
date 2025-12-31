#include "day7.h"
#include "include/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void day7(char *fileName) {
  day7Problem1(fileName);
  day7Problem2(fileName);
}


int day7Problem1(char *fileName) {
  char* data = readFile(fileName);
  Items lines = splitBy(data, '\n');

  size_t rows =  lines.count;
  size_t cols = lines.items[0]->length;

  Grid grid = initGrid(data, rows*cols, rows, cols);
  GridLocation sloc = findFirstInGrid(&grid, 'S');

  char* beams = (char*)malloc(sizeof(char) * (cols));
  memset(beams, 0, sizeof(char) * (cols));
  beams[sloc.x] = 1;

  int splits = 0;
  for (size_t i = 2; i < rows; i++) {
    if (i % 2 == 0) {
      for (size_t j = 0; j < cols; j++) {
        if (beams[j] == 1) {
          GridLocation current = initGridLocation(j, i, 1);
          if (getGridAt(&grid, &current) == '^') {
            beams[j-1] = 1;
            beams[j+1] = 1;
            beams[j] = 0;
            splits += 1;
          }
        }
      }
    }
  }
  printf("Day 7 Problem 1: %d\n", splits);

  return 0;
}

// Represent a splitter in the grid
typedef struct {
  GridLocation loc;
  int left;
  int right;
  long long totalPaths;
  size_t output;
} Splitter;

int day7Problem2(char *fileName) {
  char* data = readFile(fileName);
  Items lines = splitBy(data, '\n');

  size_t rows =  lines.count;
  size_t cols = lines.items[0]->length;

  Grid grid = initGrid(data, rows*cols, rows, cols);
  GridLocation sloc = findFirstInGrid(&grid, 'S');


  Splitter** sparseMap = (Splitter**)malloc(sizeof(Splitter*)*rows*cols);
  memset(sparseMap, 0, sizeof(Splitter)*rows*cols);
  GridLocationList splitterLocs = findAllInGrid(&grid, '^');
  for (size_t i = 0; i < splitterLocs.length; i++) {
    GridLocation loc = getGridLocationFromList(&splitterLocs, i);
    size_t idx = indexFromGridLocation(&grid, &loc);
    Splitter* splitter = (Splitter*)malloc(sizeof(Splitter));
    splitter->left = 0;
    splitter->right = 0;
    splitter->loc = loc;
    sparseMap[idx] = splitter;
  }

  Stack splitterStack = initStack(rows / 2);
  Splitter* startSplitter = (Splitter*)malloc(sizeof(Splitter));
  startSplitter->loc = sloc;
  startSplitter->left = 1;
  startSplitter->right = 1;
  startSplitter->totalPaths = 0;
  startSplitter->output = findFirstInGrid(&grid, 'S').x;
  pushStack(&splitterStack, startSplitter);

  size_t depth = 2;
  int down = 1;

  while (peekStack(&splitterStack) != NULL) {
    Splitter* currentSplitter = (Splitter*)peekStack(&splitterStack);
    // We have hit bottom. Add to the current splitter's total paths
    // and then jump back up to the source splitter by popping the stack
    // Then continue from the top of the loop
    if (depth == rows) {
      currentSplitter->totalPaths +=1;
      Splitter* prevsp = popStack(&splitterStack);
      depth = prevsp->loc.y;
      down = 0;
      continue;
    }

    // We are not at bottom check if there is a splitter at the current output
    Splitter* nextSplitter = sparseMap[depth*grid.cols + currentSplitter->output];

    // If there is a splitter inline with the output
    if (nextSplitter != NULL) {
      // If we have already visited both branches of this splitter
      // Add its total paths to the current splitter and pop back up
      if (nextSplitter->left && nextSplitter->right) {
        currentSplitter->totalPaths += nextSplitter->totalPaths;
        down = 0;
        Splitter* prevsp = popStack(&splitterStack);
        depth = prevsp->loc.y;
      // If we have not visited the left branch yet visit it
      } else if (!nextSplitter->left) {
        nextSplitter->left = 1;
        nextSplitter->output = nextSplitter->loc.x -1;
        pushStack(&splitterStack, nextSplitter);
        depth +=2;
        down = 1;
      // if we havent vistited the right branch yet visit it
      } else if (!nextSplitter->right) {
        nextSplitter->right = 1;
        nextSplitter->output = nextSplitter->loc.x +1;
        pushStack(&splitterStack, nextSplitter);
        depth +=2;
        down = 1;
      // This should be unreachable
      } else {
        panic("Logic error reached unreachable code");
      }
    // If there is no splitter inline with the output
      // but we are heading down continue down
    } else if (down) {
      depth +=2;
      down = 1;
    // If there is no splitter inline with the output
    // but we are heading up add to total paths and pop back up
    // to the previous splitter
    } else {
      Splitter* prevsp = popStack(&splitterStack);
      currentSplitter = (Splitter*)peekStack(&splitterStack);
      currentSplitter->totalPaths += prevsp->totalPaths;
      depth = prevsp->loc.y;
      down = 0;
    }
  }

  printf("Day7 Problem2: %lld\n", startSplitter->totalPaths);
  return 0;
}

