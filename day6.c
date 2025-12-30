#include "include/day6.h"
#include "../localib/include/parse.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum state {
  DIGIT = 0,
  WHITESPACE = 1,
  OP = 2
} STATE;

typedef enum mathOP {
  MUL = 1,
  ADD = 2,
} MATHOP;

typedef struct operation {
  int* vals;
} Operation;


int isDigit(char c);
int isWhiteSpace(char c);
void panic(const char*);

void day6(char* fileName) {
  day6Problem1(fileName);
  day6Problem2(fileName);
}
int day6Problem1(char* fileName) {
  char* data = readFile(fileName);
  Items lines = splitBy(data, '\n');
  size_t rows = lines.count;

  Operation* operations = (Operation *)malloc(sizeof(Operation) * 1000);
  for (size_t i = 0; i < 1000; i++) {
    operations[i].vals = (int *)malloc(sizeof(int) * rows);
  }

  for (size_t i = 0; i < rows; i++) {
    char* string = stringViewToString(lines.items[i]);
    size_t opCount = 0;
    char digit[10] = {0};
    size_t len = 0;
    STATE state = isWhiteSpace(*string) ? WHITESPACE : DIGIT;
    while (*string != '\0') {
      if (isDigit(*string)) {
        if (state == WHITESPACE) {
          len = 0;
          state = DIGIT;
        }
        digit[len] = *string;
        len++;
      } else {
        if (state == DIGIT) {
          assert(len < 10);
          digit[len] = '\0';
          operations[opCount].vals[i] = atoi(digit);
          opCount++;
          state = WHITESPACE;
          memset(digit, 0, sizeof(digit));
        }
      }
      string++;
    }
    assert(len < 10);
    digit[len] = '\0';
    operations[opCount].vals[i] = atoi(digit);
    opCount++;
    state = WHITESPACE;
    memset(digit, 0, sizeof(digit));
  }

  long long sum = 0;
  long long product;
  char* ops = stringViewToString(lines.items[rows]);
  size_t opCount = 0;
  while (*ops != '\0') {
    if (!isWhiteSpace(*ops)) {
      switch (*ops) {
        case '+':
          for (size_t i = 0; i < rows; i++) {
            sum += operations[opCount].vals[i];
          }
          break;
        case '*':
          product = 1;
          for (size_t i = 0; i < rows; i++) {
            product *= operations[opCount].vals[i];
          }
          sum += product;
          break;
        default:
          panic("Illegal OP");
      }
      opCount++;
    }
    ops++;
  }
  printf("day6Problem1: %lld\n", sum);

  return 0;
}
int day6Problem2(char* fileName) {
  char* data = readFile(fileName);
  Items lines = splitBy(data, '\n');
  size_t rows = lines.count;
  long long total = 0;

  const StringView* opsRow = lines.items[rows];
  size_t opsLen = opsRow->length;

  char** digits = (char**)malloc(sizeof(char*) * 10);
  for (size_t i = 0; i < 10; i++) {
    digits[i] = (char*)malloc(sizeof(char) * rows);
    memset(digits[i], 0, sizeof(char) * rows);
  }
  size_t opStart = 0;
  size_t digitCount = 0;
  for (size_t i = 0; i <= opsLen; i++) {
    // If we found a new op or we are at the end of the line
    if (opsRow->str[i] == '+' || opsRow->str[i] == '*') {
      // the first op we dont need to sum the previous digits
      if (i == 0) {
        continue;
      }
      for (size_t j = 0; j < digitCount; j++) {
        for (size_t k = 0; k < rows; k++) {
          digits[j][k] = lines.items[k]->str[opStart + j];
        }
      }
      long long product;
      switch (opsRow->str[opStart]) {
        case '+':
          for (size_t j = 0; j < 10; j++) {
            total += atoi(digits[j]);
          }
          break;
        case '*':
          product = 1;
          for (size_t j = 0; j < 10; j++) {
            if (digits[j][0] == '\0') {
              continue;
            }
            product *= atoi(digits[j]);
          }
          total += product;
          break;
        default:
          panic("Illegal OP");
      }
      digitCount = 0;
      opStart = i;
      for (size_t i = 0; i < 10; i++) {
        digits[i] = (char*)malloc(sizeof(char) * rows);
        memset(digits[i], 0, sizeof(char) * rows);
      }
    } else {
      digitCount++;
    }
  }
  digitCount++;
  for (size_t j = 0; j < digitCount; j++) {
    for (size_t k = 0; k < rows; k++) {
      digits[j][k] = lines.items[k]->str[opStart + j];
    }
  }
  long long product;
  switch (opsRow->str[opStart]) {
    case '+':
      for (size_t j = 0; j < 10; j++) {
        total += atoi(digits[j]);
      }
      break;
    case '*':
      product = 1;
      for (size_t j = 0; j < 10; j++) {
        if (digits[j][0] == '\0') {
          continue;
        }
        product *= atoi(digits[j]);
      }
      total += product;
      break;
    default:
      panic("Illegal OP");
  }

  printf("day6Problem2: %lld\n", total);
  return 0;
}

int isDigit(char c) {
  return c >= '0' && c <= '9';
}

int isWhiteSpace(char c) {
  return c == '\n' || c == ' ';
}

void panic(const char* message) {
    fprintf(stderr, "panic: %s\n", message);
    exit(1);
}
