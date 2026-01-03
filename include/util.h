#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GRID_LOCATION_DISPLAY_SIZE 128

#define PRINT_PTR_LIST(arr, len,  typ) { \
  printf("length: %zu {", len); \
  for (size_t i = 0; i < len; i++) { \
      printf("%d", *(typ)arr[i]); \
      if (i < len-1) \
        printf(", "); \
  } \
  printf("}\n"); \
}


typedef struct {
  const char *str;
  size_t length;
} StringView;

typedef struct {
  const StringView **items;
  size_t count;
} Items;

typedef struct {
  size_t len;
  size_t cap;
  char** data;
} dynStringArray;

typedef struct {
  char* items;
  size_t rows;
  size_t cols;
} Grid;

typedef struct {
  size_t x;
  size_t y;
  int exists;
} GridLocation;

typedef struct {
  GridLocation* locs;
  size_t length;
  size_t cap;
} GridLocationList;

typedef struct stack {
  void** items;
  size_t length;
  size_t cap;
} Stack;


void _stringArrayExtend(dynStringArray*);

dynStringArray stringArrayInit(size_t);
void _dynStringArrayExtend(dynStringArray*);

char* readFile(char*);

size_t _countItems(char*, char);

Items splitBy(char*, char);

char* stringViewToString(const StringView*);


Grid initGrid(char*, size_t, size_t, size_t);
char getGridAt(const Grid*, const GridLocation*);
void setGridAt(Grid*, const GridLocation*, char);
GridLocation findFirstInGrid(const Grid*, char);
GridLocationList findAllInGrid(const Grid*, char);
GridLocation _locFromGridIndex(const Grid*, size_t);
size_t indexFromGridLocation(const Grid*, GridLocation*);

GridLocation initGridLocation(size_t, size_t, int);
char* displayGridLocation(GridLocation*);

GridLocationList initGridLocationList(size_t);
char* displayGridLocationList(GridLocationList*);
void appendToGridLocationList(GridLocationList*, GridLocation);
void setGridLocationListAt(GridLocationList*, size_t, GridLocation);
GridLocation popGridLocationFromList(GridLocationList*);
GridLocation getGridLocationFromList(GridLocationList*, size_t);


Stack initStack(size_t);
void* popStack(Stack*);
void pushStack(Stack*, void*);
void* peekStack(Stack*);

void panic(const char*);

void quickSort(void**, size_t, size_t, size_t, int (*)(void*, void*));
void swap(void**, void**);
