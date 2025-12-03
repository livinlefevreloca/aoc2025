#include "include/day1.h"
#include "include/day2.h"
#include "include/parse.h"
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
      default:
          printf("Day %d is not implemented yet.\n", day);
          break;
    }
    return 0;
}
