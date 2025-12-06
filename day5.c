#include "include/day5.h"

void day5(char *fileName) {
  day5Problem1(fileName);
  day5Problem2(fileName);
}

int day5Problem1(char *fileName) {
  char* data = readFile(fileName);
  Items views = splitBy(data, '\n');

  size_t rangesCount = 0;
  for (size_t i = 0; i < views.count; i++) {
    if (!containsDash(views.items[i])) {
      rangesCount = i;
      break;
    }
  };

  Range* ranges = (Range*)malloc(rangesCount * sizeof(Range));
  for (size_t i = 0; i < rangesCount; i++) {
    ranges[i] = parseRange(stringViewToString(views.items[i]));
  };

  size_t numbersCount = views.count - rangesCount;
  int64_t* numbers = (int64_t*)malloc(numbersCount*sizeof(int));
  for (size_t i = rangesCount; i < views.count; i++) {
    numbers[i-rangesCount] = atol(stringViewToString(views.items[i]));
  }

  int fresh = 0;
  for (size_t i = 0; i < numbersCount; i++) {
    for (size_t j = 0; j < rangesCount; j++) {
      if (rangeContains(ranges[j], numbers[i])) {
        fresh++;
        break;
      }
    }
  }

  printf("Day5 Problem1: %d\n", fresh);
  return 0;
}

int day5Problem2(char *fileName) {
  char* data = readFile(fileName);
  Items views = splitBy(data, '\n');

  size_t rangesCount = 0;
  for (size_t i = 0; i < views.count; i++) {
    if (!containsDash(views.items[i])) {
      rangesCount = i;
      break;
    }
  };

  Range* ranges = (Range*)malloc(rangesCount * sizeof(Range));
  for (size_t i = 0; i < rangesCount; i++) {
    ranges[i] = parseRange(stringViewToString(views.items[i]));
  };

  Range* sortedRanges = (Range*)malloc(rangesCount * sizeof(Range));
  sortedRanges[0] = ranges[0];
  size_t sortedCount = 1;
  for (size_t i = 1; i < rangesCount; i++) {
    Range newRange = ranges[i];
    for (size_t j = 0; j <= sortedCount; j++) {
      // end reached, append
      if (j == sortedCount) {
        sortedRanges[j] = newRange;
        sortedCount++;
        break;
      }

      Range sortedRange = sortedRanges[j];
      // range is before the current sorted range. insert and shift all to right
      if (newRange.high < sortedRange.low) {
        sortedRanges[j] = newRange;
        // move all to left by 1
        for (size_t k = j+1; k <= sortedCount; k++) {
          newRange = sortedRanges[k];
          sortedRanges[k] = sortedRange;
          sortedRange = newRange;
        }
        sortedCount++;
        break;
      // the ranges overlap merge them and all others that overlap
      } else if (rangeOverlaps(newRange, sortedRange)) {
        newRange = mergeRanges(newRange, sortedRange);
        sortedRanges[j] = newRange;
        size_t k = j+1;
        while (k < sortedCount && rangeOverlaps(newRange, sortedRanges[k])) {
          newRange = mergeRanges(newRange, sortedRanges[k]);
          sortedRanges[j] = newRange;
          for (size_t l = k; l < sortedCount; l++) {
            sortedRanges[l] = sortedRanges[l+1];
          }
          k++;
          // we've now removed one range decrease count
          sortedCount--;
        }
        break;
      }
    }
  }

  for (size_t i = 0; i < sortedCount-1; i++) {
    assert(sortedRanges[i].high < sortedRanges[i+1].low);
  }

  int64_t total = 0;
  for (size_t i = 0; i < sortedCount; i++) {
    total += (sortedRanges[i].high - sortedRanges[i].low) + 1;
  }

  printf("Day5 Problem2: %lld\n", total);

  return 0;
}

Range mergeRanges(Range range1, Range range2) {
  Range range;
  range.low = MIN(range1.low, range2.low);
  range.high = MAX(range1.high, range2.high);

  return range;
}

int rangeOverlaps(Range range1, Range range2) {
    return (range1.low <= range2.low && range1.high <= range2.high && range1.high >= range2.low)
      || (range1.low >= range2.low && range1.high <= range2.high)
      || (range2.low >= range1.low && range2.high <= range1.high)
      || (range1.low >= range2.low && range1.high >= range2.high && range1.low <= range2.high);
}

Range parseRange(char* rangeStr) {
  Items parts = splitBy(rangeStr, '-');
  Range range;
  range.low = atol(stringViewToString(parts.items[0]));
  range.high = atol(stringViewToString(parts.items[1]));

  return range;
}

int rangeContains(Range range, int64_t num) {
  return (range.high > num && range.low < num) || range.low == num || range.high ==  num;
}


int containsDash(const StringView* view) {
  for (size_t i = 0; i < view->length; i++) {
    if (view->str[i] == '-') {
      return 1;
    }
  }
  return 0;
}
