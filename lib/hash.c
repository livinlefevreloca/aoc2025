#include <stdlib.h>
#include "../include/dll.h"


typedef size_t(*intHashFunction)(int);
typedef size_t(*charHashFunction)(char);
typedef size_t(*strHashFunction)(char*);

typedef union hashFunction {
  intHashFunction intHash;
  charHashFunction charHash;
  strHashFunction strHash;
} HashFunction;


typedef struct hashMap {
  size_t size;
  size_t cap;

  DLList** buckets;


  HashFunction hash;
} HashMap;
