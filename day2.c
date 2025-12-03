#include "include/parse.h"
#include "include/day2.h"

void day2(char *fileName) {
  day2Problem1(fileName);
  day2Problem2(fileName);
}

int day2Problem1(char *fileName) {
  char * data = readFile(fileName);
  Items items = splitBy(data, ',');
  int64_t sum = 0;
  char buffer[128];
  memset((char *)&buffer, 0, sizeof(buffer));
  for (int i = 0; i < items.count; i++) {
    strncpy((char *)&buffer, items.items[i]->str, items.items[i]->length);
    sum += sumRange((char *)buffer);
    memset((char *)&buffer, 0, sizeof(buffer));
  }

  printf("Problem1: %lld\n", sum);
  return 0;
}

int day2Problem2(char *fileName) {
  char * data = readFile(fileName);
  Items items = splitBy(data, ',');
  int64_t sum = 0;
  char buffer[128];
  memset((char *)&buffer, 0, sizeof(buffer));
  for (int i = 0; i < items.count; i++) {
    strncpy((char *)&buffer, items.items[i]->str, items.items[i]->length);
    sum += sumRange2((char *)buffer);
    memset((char *)&buffer, 0, sizeof(buffer));
  }

  printf("Problem2: %lld\n", sum);
  return 0;
}

int64_t sumRange(char *range) {
  int64_t sum = 0;
  char start[32];
  char end[32];
  memset((char *)&start, 0, sizeof(start));
  memset((char *)&end, 0, sizeof(end));
  Items parts = splitBy(range, '-');

  strncpy((char *)&start, parts.items[0]->str, parts.items[0]->length);
  strncpy((char *)&end, parts.items[1]->str, parts.items[1]->length);
  // printf("Start: %s, End: %s\n", start, end);
  int64_t s = atol(start);
  int64_t e = atol(end);

  // printf("Range: %d - %d, spower: %d\n", s, e, spower);
  char buffer[32];
  for (int64_t i = s; i <= e; i++) {
    snprintf((char *)&buffer, 32, "%ld", (long)i);
    if (strlen(buffer) % 2 != 0) {
      continue;
    }
    int spower = strlen(buffer) / 2;
    // printf("Checking number: %d, spower: %d\n", i, spower);
    int64_t div = (int64_t)pow(10.0, (double)spower);
    // printf("Divisor: %d\n", div);
    if (i / div == i % div) {
      sum += i;
    }
  }

  return sum;
}

int64_t sumRange2(char* range) {

  int64_t sum = 0;
  char start[32];
  char end[32];
  memset((char *)&start, 0, sizeof(start));
  memset((char *)&end, 0, sizeof(end));
  Items parts = splitBy(range, '-');

  strncpy((char *)&start, parts.items[0]->str, parts.items[0]->length);
  strncpy((char *)&end, parts.items[1]->str, parts.items[1]->length);
  int64_t s = atol(start);
  int64_t e = atol(end);

  char buffer[32];
  for (int64_t i = s; i <= e; i++) {
    snprintf((char *)&buffer, 32, "%ld", (long)i);
    int numLen = strlen(buffer);
    int** factorsArr = (int**)malloc((numLen/2) * sizeof(int*));
    factors(numLen, factorsArr);
    for (int j = 0; j < numLen/2; j++) {
      if (factorsArr[j] == NULL) {
        break;
      }
      if (checkPattern((char* )&buffer, *factorsArr[j], numLen)) {
        sum += i;
        free(factorsArr[j]);
        break;
      }
      free(factorsArr[j]);
    }
    free(factorsArr);
  }
  return sum;
}

void factors(int num, int** factors) {
  int factorIndex = 0;
  for (int i = 1; i <= num/2; i++) {
    if (num % i == 0) {
      int* factor = (int*)malloc(sizeof(int));
      *factor = i;
      factors[factorIndex] = factor;
      factorIndex++;
    }
  }
}

int checkPattern(char* num, int window, int len) {
  char *first = num;
  for (int i = window; i <= len-window ; i+= window) {
    int different = memcmp((void *)&first[i-window], (void*)&num[i], window);
    if (different) {
      return 0;
    }
  }

  return 1;
}


