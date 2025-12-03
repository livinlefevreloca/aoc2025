#include "include/parse.h"
#include "include/day1.h"
#include <stdio.h>
#include <string.h>

void day1(char* fileName) {
  day1Problem1(fileName);
  day1Problem2(fileName);
}

int day1Problem1(char* fileName) {
  char *fileData = readFile(fileName);
  Items items = splitBy(fileData, '\n');
  int current = 50;
  int zCount = 0;

  char str[32];
  memset(str, 0, sizeof(str));
  for (size_t i = 0; i < items.count; i++){
    const StringView *strv = items.items[i];
    strncpy((char*)str, strv->str, strv->length);
    str[strv->length] = '\0';
    int ticks = getTicks((char *)str);
    current += (ticks % 100);
    current %= 100;
    if (current == 0) {
      zCount++;
    }
    memset(str, 0, sizeof(str));
  }

  printf("Problem1: %d\n", zCount);

  return 0;
}

int day1Problem2(char* fileName) {
  char *fileData = readFile(fileName);
  Items items = splitBy(fileData, '\n');
  int current = 50;
  int zCount = 0;

  char str[32];
  memset(str, 0, sizeof(str));
  for (size_t i = 0; i < items.count; i++){
    const StringView *strv = items.items[i];
    strncpy((char*)str, strv->str, strv->length);
    str[strv->length] = '\0';
    int oldCurrent = current;
    int ticks = getTicks((char *)str);
    int turns = abs(ticks) / 100;
    int remainder = ticks % 100;
    current += remainder;
    zCount += turns + 1*((current >= 100 || current <= 0) && oldCurrent != 0);
    current = current + 100*(current < 0) - 100*(current >= 100);
    assert(current >= 0 && current < 100);
    memset(str, 0, sizeof(str));
  }

  printf("Problem2: %d\n", zCount);

  return 0;
}

int getTicks(char* code) {
  char direction = code[0];
  int amount = atoi(&code[1]);

  if (direction == 'R') {
    return amount;
  } else {
    return -amount;
  }
}
