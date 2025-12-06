#include "include/day4.h"
#include <stdio.h>
#include <string.h>
#include <localib/parse.h>

void day4(const char *inputFile) {
  day4Problem1(inputFile);
  day4Problem2(inputFile);
}

int day4Problem1(const char *inputFile) {
  char *data = readFile((char*)inputFile);
  int len = strlen(data);
  // +1 to account for newline char
  int rowLen = splitBy(data, '\n').items[0]->length + 1;
  int total = 0;

  for (int i=0; i < len; i++) {
    if (data[i] != '@') {
      continue;
    }
    int rolls = 0;
    int dirs[8] = {
      i % rowLen == 0 ? -1 : i -rowLen -1,
      i - rowLen,
      i % rowLen == rowLen -1 ? -1 : (i - rowLen) + 1,
      i % rowLen == 0 ? -1 : i - 1,
      i % rowLen == rowLen -1 ? -1 : i + 1,
      i % rowLen == 0 ? -1 : i + rowLen - 1,
      i + rowLen,
      i % rowLen == rowLen -1 ? -1 : i + rowLen + 1,
    };
    for (int j = 0; j < 8; j++) {
      if (rolls >= 4) {
        break;
      }
      int dir = dirs[j];
      if (dir < 0 || dir > len) {
        continue;
      }

      if (data[dir] == '@') {
        rolls++;

      }
    }
    if (rolls < 4) {
      total++;
    }
  }
  printf("Day 4, Problem 1: %d\n", total);

  return 0;
}

int day4Problem2(const char *inputFile) {
  char *data = readFile((char*)inputFile);
  int len = strlen(data);
  // +1 to account for newline char
  int rowLen = splitBy(data, '\n').items[0]->length + 1;
  int total = 0;
  int oldTotal = -1;
  while (oldTotal < total) {
    oldTotal = total;
    for (int i=0; i < len; i++) {
      if (data[i] != '@') {
        continue;
      }
      int rolls = 0;
      int dirs[8] = {
        i % rowLen == 0 ? -1 : i -rowLen -1,
        i - rowLen,
        i % rowLen == rowLen -1 ? -1 : (i - rowLen) + 1,
        i % rowLen == 0 ? -1 : i - 1,
        i % rowLen == rowLen -1 ? -1 : i + 1,
        i % rowLen == 0 ? -1 : i + rowLen - 1,
        i + rowLen,
        i % rowLen == rowLen -1 ? -1 : i + rowLen + 1,
      };
      for (int j = 0; j < 8; j++) {
        if (rolls >= 4) {
          break;
        }
        int dir = dirs[j];
        if (dir < 0 || dir > len) {
          continue;
        }

        if (data[dir] == '@') {
          rolls++;
        }
      }
      if (rolls < 4) {
        total++;
        data[i] = '.';
      }
    }
  }
  printf("Day 4, Problem 2: %d\n", total);
  return 0;
}
