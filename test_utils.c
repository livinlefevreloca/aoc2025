#include "include/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_test(int (*)(void), char*);
int compare(void*, void*);
int test_swap(void);
int test_quickSort_simple(void);
int test_quickSort_random(void);
int test_quickSort_object(void);
int test_quickSort_object_large(void);

int main(void) {
  run_test(test_swap, "test_swap");
  run_test(test_quickSort_simple, "test_quickSort");
  run_test(test_quickSort_random, "test_quickSort_random");
  run_test(test_quickSort_object, "test_quickSort_object");
  run_test(test_quickSort_object_large, "test_quickSort_object_large");
  return 0;

}

void run_test(int (*test)(void), char* name) {
  printf("Running test: %s...\n", name);
  int res = test();
  if (!res) {
    printf("Test %s failed!\n", name);
    return;
  }
  printf("Test %s passed!\n", name);
}

int compare(void* a, void* b) {
  return (*(int*)a) - (*(int*)b);
}

int test_swap(void) {
  int a = 5;
  int b = 1;
  int* ap = &a;
  int *bp = &b;
  swap((void**)&ap, (void**)&bp);
  if (*ap != 1 || *bp != 5) {
    printf("swap failed: a=%d, b=%d\n", a, b);
    return 0;
  }
  return 1;
}

int _compare_int(void* a, void* b) {
  return (*(int*)a) - (*(int*)b);
}

int test_quickSort_simple(void) {
  int data[] = {2, 5, 3, 1, 4};
  int* arr[] = {&data[0], &data[1], &data[2], &data[3], &data[4]};
  quickSort((void*)arr, 5, 0, 5, _compare_int);
  for (int i = 0; i < 5; i++) {
    if (*arr[i] != i+1) {
      printf("quickSort failed at index %d: expected %d, got %d\n", i, i+1, data[i]);
      printf("After sorting: ");
      PRINT_PTR_LIST(arr, 5ul, int*);
      return 0;
    }
  }
  return 1;
}


int test_quickSort_random(void) {
  int** arr = (int**)malloc(sizeof(int*) * 1000);
  for (size_t i = 0; i < 1000; i++) {
    arr[i] = (int*)malloc(sizeof(int));
    *arr[i] = rand() % 10000;
  }

  quickSort((void*)arr, 1000, 0, 1000, _compare_int);

  for (size_t i = 1; i < 1000; i++) {
    if (*arr[i-1] > *arr[i]) {
      printf("quickSort failed at index %zu: %d > %d\n", i, *arr[i-1], *arr[i]);
      printf("After sorting: ");
      PRINT_PTR_LIST(arr, 1000ul, int*);
      for (size_t j = 0; j < 1000; j++) {
        free(arr[j]);
      }
      free(arr);
      return 0;
    }
  }

  return 1;
}

struct complex {
  char value[20];
  int key;
};

int _compare_complex(void* a, void* b) {
  struct complex* ca = (struct complex*)a;
  struct complex* cb = (struct complex*)b;
  return ca->key - cb->key;
}

int test_quickSort_object(void) {

  struct complex** arr = (struct complex**)malloc(sizeof(struct complex*) * 1000);
  for (size_t i = 0; i < 1000; i++) {
    arr[i] = (struct complex*)malloc(sizeof(struct complex*));
    arr[i]->key = rand() % 10000;
    strcpy(arr[i]->value, "abdsb");
  }

  quickSort((void**)arr, 1000, 0, 1000, _compare_complex);

  for (size_t i = 1; i < 1000; i++) {
    if (arr[i-1]->key > arr[i]->key) {
      printf("quickSort failed at index %zu: %d > %d\n", i, arr[i-1]->key, arr[i]->key);
      printf("After sorting: ");
      PRINT_PTR_LIST(arr, 1000ul, int*);
      for (size_t j = 0; j < 1000; j++) {
        free(arr[j]);
      }
      free(arr);
      return 0;
    }
  }

  return 1;
}


int test_quickSort_object_large(void) {
  size_t counts[]  = {1000, 10000, 100000, 1000000};
  for (size_t count_index = 0; count_index < sizeof(counts)/sizeof(counts[0]); count_index++) {
    printf("Testing quickSort with %zu elements...\n", counts[count_index]);
    size_t count = counts[count_index];
    struct complex** arr = (struct complex**)malloc(sizeof(struct complex*) * count);
    for (size_t i = 0; i < count; i++) {
      arr[i] = (struct complex*)malloc(sizeof(struct complex*));
      arr[i]->key = rand() % count*10;
      strcpy(arr[i]->value, "abdsb");
    }

    quickSort((void**)arr, count, 0, count, _compare_complex);

    for (size_t i = 1; i < count; i++) {
      if (arr[i-1]->key > arr[i]->key) {
        printf("quickSort failed at index %zu: %d > %d\n", i, arr[i-1]->key, arr[i]->key);
        printf("After sorting: ");
        PRINT_PTR_LIST(arr, count, int*);
        for (size_t j = 0; j < count; j++) {
          free(arr[j]);
        }
        free(arr);
        return 0;
      }
    }
    for (size_t j = 0; j < count; j++) {
      free(arr[j]);
    }
    free(arr);
  }
  return 1;
}
