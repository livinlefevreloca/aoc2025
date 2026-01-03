#include "include/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* readFile(char *path) {
  int fh = open(path, O_RDONLY);

  if (fh == -1) {
    char buf[1024];
    snprintf(buf, 1024, "Failed to open file: %s\n", path);
    perror(buf);
    return "";
  }

  struct stat v;
  stat(path, &v);
  int size =  v.st_size;

  char* data = (char*)malloc(size*sizeof(char) +1);

  if (data == NULL) {
    perror("OUT OF MEMORY");
    return "";
  }

  int bytes = read(fh, data, size);
  if (bytes == -1) {
    char buf[1024];
    snprintf(buf, 1024, "Failed to read file: %s\n", path);
    perror(buf);
    return "";
  }
  data[size] = '\0';
  return data;
}

size_t _countItems(char *data, char delim) {
  size_t count = 0;
  int idx = 0;
  while (data[idx] != '\0') {
    if (data[idx] == delim) {
      count++;
    }
    idx++;
  }

  // If the last char is a newline reduce the count by 1
  if (data[idx-1] == delim) {
    count--;
  }

  return count;
}

Items splitBy(char *data, char delim) {
  int idx = 0;
  size_t itemsSize = _countItems(data, delim)+1;

  StringView** views = (StringView**)malloc(itemsSize*sizeof(StringView*));
  if (views == NULL) {
    perror("OUT OF MEMORY splitBy on items");
  }

  size_t viewsIdx = 0;
  size_t curLen = 0;
  while (data[idx] != '\0') {
    if (data[idx] == delim) {
      // empty line
      if (curLen == 0) {
        idx++;
        continue;
      }
      StringView* view = (StringView*)malloc(sizeof(StringView));
      view->str = &data[idx-curLen];
      view->length = curLen;
      views[viewsIdx] = view;

      curLen = 0;
      viewsIdx++;
    } else {
      curLen++;
    }
    idx++;
  }
  StringView* view = (StringView*)malloc(sizeof(StringView));
  view->str = &data[idx-curLen];
  view->length = curLen;
  views[viewsIdx] = view;

  assert(viewsIdx <= itemsSize);

  Items items;
  items.items = (const StringView**)views;
  items.count = viewsIdx;


  return items;
}

char* stringViewToString(const StringView* view) {
  char* string = (char*)malloc(sizeof(char)*view->length + 1);
  strncpy(string, view->str, view->length);
  string[view->length] = '\0';
  return string;
}

void stringArrayPush(dynStringArray* arr, char* val) {
  if (arr->len == arr->cap) {
    _dynStringArrayExtend(arr);
  }
  arr->data[arr->len] = val;
  arr->len++;
}

void _dynStringArrayExtend(dynStringArray* arr) {
  char** newData = (char**)malloc(sizeof(char*)*arr->cap*2);
  arr->cap =  arr->cap*2;
  memcpy(newData, arr->data, arr->len);
  char** oldData = arr->data;
  arr->data = newData;
  free(oldData);
}

char* dynStringArrayPop(dynStringArray* arr) {
  if (arr->len <= 0) {
    return NULL;
  }

  arr->len--;
  return arr->data[arr->len];
}


Grid initGrid(char* data, size_t length, size_t rows, size_t columns) {
  Grid grid;
  assert(rows*columns >= length);
  char* items = (char*)malloc(sizeof(char)*rows*columns);
  grid.rows = rows;
  grid.cols = columns;

  int offset = 0;
  for (size_t i = 0; i < rows*columns; i++) {
    if (data[i] == '\n') {
      offset++;
      continue;
    }
    if (i >= length) {
      items[i-offset] = '\0';
      continue;
    }
    items[i-offset] = data[i];
  }

  grid.items = items;
  return grid;
}

char getGridAt(const Grid* grid, const GridLocation* loc) {
  size_t idx = loc->y*grid->cols + loc->x;
  return grid->items[idx];
}

void setGridAt(Grid* grid, const GridLocation* loc, char c) {
  size_t idx = loc->y*grid->cols + loc->x;
  grid->items[idx] = c;
}

GridLocation findFirstInGrid(const Grid* grid, char c) {
  for (size_t i = 0; i < grid->rows*grid->cols; i++) {
    if (grid->items[i] == c) {
      return _locFromGridIndex(grid, i);
    }
  }
  return initGridLocation(0, 0, 0);
}

GridLocationList findAllInGrid(const Grid * grid, char c) {
  GridLocationList list = initGridLocationList(64);

  for (size_t i = 0; i < grid->rows*grid->cols; i++) {
    if (grid->items[i] == c) {
      GridLocation loc = _locFromGridIndex(grid, i);
      appendToGridLocationList(&list, loc);
    }
  }

  return list;
}

GridLocation _locFromGridIndex(const Grid* grid, size_t idx) {
  size_t x = idx % grid->cols;
  size_t y = idx / grid->cols;
  GridLocation loc = initGridLocation(x, y, 1);
  return loc;
}

size_t indexFromGridLocation(const Grid* grid, GridLocation* loc) {
  return loc->y*grid->cols + loc->x;
}


GridLocation initGridLocation(size_t x, size_t y, int exists) {
  GridLocation loc;
  loc.x = x;
  loc.y = y;
  loc.exists = exists;
  return loc;
}

char* displayGridLocation(GridLocation* loc) {
  char* str = (char*)malloc(GRID_LOCATION_DISPLAY_SIZE);
  if (loc->exists) {
    snprintf(str, GRID_LOCATION_DISPLAY_SIZE, "GridLocation(x=%zu, y=%zu)", loc->x, loc->y);
  } else {
    snprintf(str, GRID_LOCATION_DISPLAY_SIZE, "GridLocation(does not exist)");
  }
  return str;
}

GridLocationList initGridLocationList(size_t size) {
  GridLocationList list;

  list.length = 0;
  list.cap = size;

  GridLocation* locations = (GridLocation*)malloc(sizeof(GridLocation)*list.cap);
  list.locs = locations;

  return list;
}

void appendToGridLocationList(GridLocationList* list, GridLocation loc) {
  if (list->length == list->cap - 1) {
    list->cap = list->cap*2;
    GridLocation* locations = (GridLocation*)malloc(sizeof(GridLocation)*list->cap);
    memcpy(locations, list->locs, list->length*sizeof(GridLocation));
    free(list->locs);
    list->locs = locations;
  }
  list->locs[list->length] = loc;
  list->length += 1;
}

void setGridLocationListAt(GridLocationList* list, size_t idx, GridLocation loc) {
  list->locs[idx] = loc;
}

GridLocation popGridLocationFromList(GridLocationList* list) {
  GridLocation loc = list->locs[list->length-1];
  list->length -= 1;
  return loc;
}

GridLocation getGridLocationFromList(GridLocationList* list, size_t idx) {
  return list->locs[idx];
}

char* displayGridLocationList(GridLocationList * list) {
  char* str = (char*)malloc(GRID_LOCATION_DISPLAY_SIZE*list->length+list->length*2+2);
  char* strPtr = str;
  *strPtr = '[';
  strPtr++;
  for (size_t i = 0; i < list->length; i++) {
    char* locStr = displayGridLocation(&list->locs[i]);
    size_t locStrLen = strlen(locStr);
    strncat(strPtr, locStr, locStrLen);
    strPtr+= locStrLen;
    if (i != list->length-1) {
      strncat(strPtr, ", ", 2);
      strPtr += 2;
    }
  }
  strncat(strPtr, "]\0", 2);
  return str;
}

Stack initStack(size_t size) {
  Stack stack;
  void** items = (void**)malloc(size*sizeof(void*));
  stack.items = items;
  stack.cap = size;
  stack.length = 0;
  return stack;
}

void* popStack(Stack* stack) {
  if (stack->length == 0) {
    return NULL;
  }
  stack->length--;
  return stack->items[stack->length];
}

void* peekStack(Stack* stack) {
  if (stack->length == 0) {
    return NULL;
  }
  return stack->items[stack->length-1];
}

void pushStack(Stack* stack, void* item) {
  if (stack->length == stack->cap) {
    size_t newCap = stack->cap * 2;
    void* newItems = (void*)malloc(newCap * sizeof(void*));
    memcpy(newItems, stack->items, stack->length * sizeof(void*));
    free(stack->items);
    stack->items = newItems;
    stack->cap = newCap;
  }
  stack->items[stack->length] = item;
  stack->length++;
}

void panic(const char* message) {
  fprintf(stderr, "PANIC: %s\n", message);
  exit(1);
}

void swap(void** a, void** b) {
  *a = (void*)((size_t)(*a) ^ (size_t)(*b));
  *b = (void*)((size_t)(*a) ^ (size_t)(*b));
  *a = (void*)((size_t)(*a) ^ (size_t)(*b));
}

void quickSort(
  void** list,
  size_t length,
  size_t start,
  size_t end,
  int (*comparator)(void*, void*)) {

  if (length == 1) {
    return;
  }

  size_t pivotIndex = end - 1;
  if (pivotIndex == 1) {
    if (comparator(list[0], list[1]) > 0) {
      swap(&list[0], &list[1]);
    }
  }

  while (comparator(list[pivotIndex-1], list[pivotIndex]) > 0) {
    swap(&list[pivotIndex-1],&list[pivotIndex]);
    pivotIndex--;
    if (pivotIndex == 0) {
      break;
    }
  }

  for (size_t i = start; i < pivotIndex; i++) {
    while (comparator(list[i], list[pivotIndex]) > 0 && i < end) {
      for (size_t j = i; j < pivotIndex; j++) {
          swap(&list[j], &list[j+1]);
      }
      pivotIndex--;
    }
  }

  if (pivotIndex-start > 0)
    quickSort(list, pivotIndex-start, start, pivotIndex, comparator);
  if (end-pivotIndex > 0)
    quickSort(list, end-pivotIndex, pivotIndex, end, comparator);
}

