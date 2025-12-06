#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <localib/parse.h>

#define MIN(a, b) a < b ? a : b;
#define MAX(a, b) a > b ? a : b;

void day5(char* fileName);
int day5Problem1(char* fileName);
int day5Problem2(char* fileName);

typedef struct range {
  int64_t low;
  int64_t high;
} Range;

int containsDash(const StringView*);
Range parseRange(char *);
int rangeContains(Range, int64_t);
int rangeOverlaps(Range, Range);
Range mergeRanges(Range, Range);


