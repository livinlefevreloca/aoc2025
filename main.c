#include "include/day1.h"
#include "include/day2.h"
#include "include/day3.h"
#include "include/day4.h"
#include "include/day5.h"
#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <day> <input_file>\n", argv[0]);
        return 1;
    }

    int day = atoi(argv[1]);
    char *inputFile = argv[2];
    switch (day) {
      case 1:
        day1(inputFile);
        break;
      case 2:
        day2(inputFile);
        break;
      case 3:
        day3(inputFile);
        break;
      case 4:
        day4(inputFile);
        break;
      case 5:
        day5(inputFile);
        break;
      default:
          printf("Day %d is not implemented yet.\n", day);
          break;
    }
    return 0;
}
