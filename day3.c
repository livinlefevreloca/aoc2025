#include "include/day3.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void day3(char* fileName) {
  day3Problem1(fileName);
  day3Problem2(fileName);

}

int day3Problem1(char* fileName) {
  char* data = readFile(fileName);
  Items items = splitBy(data, '\n');
  int sum = 0;

  for (size_t i = 0; i < items.count; i++) {
    sum += findMaxCombination(items.items[i]);
  }

  printf("Problem1: %d\n", sum);
  return 0;
}

int findMaxCombination(const StringView* view) {
  int highest = 0;
  int nextHighest = 0;
  for (size_t i = 0; i < view->length; i++) {
    int oldHighest = highest;
    highest = MAX(highest, (int)view->str[i]-48);
    if (i+1 < view->length) {
      if (highest != oldHighest) {
        nextHighest = view->str[i+1]-48;
      } else {
        nextHighest = MAX(nextHighest, view->str[i+1]-48);
      }
    } else {
      highest = oldHighest;
    }
  }
  return highest * 10 + nextHighest;
}

int day3Problem2(char* fileName) {
  char* data = readFile(fileName);
  Items items = splitBy(data, '\n');
  int64_t sum = 0;
  for (size_t i = 0; i < items.count; i++) {
    sum += findMaxCombination2(items.items[i]);
  }

  printf("Problem2: %ld\n", (long)sum);
  return 0;
}

int64_t findMaxCombination2(const StringView *view) {
  int digits[12];
  memset(digits, 0, sizeof(digits));
  size_t idx = 0;
  for (size_t i = 0; i < 12; i++) {
    int max = view->str[idx]-48;
    size_t maxIndex = 0;
    for (size_t j = idx+1; j < view->length; j++) {
      if (view->length - j < 12 - i) {
        break;
      }
      int oldMax = max;
      max = MAX(max, view->str[j]-48);
      if (max != oldMax) {
        maxIndex = j;
      }
    }
    digits[i] = max;
    idx = maxIndex > idx ? maxIndex + 1 : idx + 1;
  }

  int64_t num = 0;
  for (size_t i = 12; i > 0; i--){
    num += (int64_t)pow(10, (double)i-1)*digits[12-i];
  }
  return num;
}


