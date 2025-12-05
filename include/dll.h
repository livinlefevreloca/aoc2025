#include <stdlib.h>

typedef struct dllNode {
  void* value;
  struct dllNode* next;
  struct dllNode* prev;
} DLLNode;

typedef DLLNode* (*NodeEqFunc)(DLLNode*, void*);
typedef struct dlList {
  DLLNode* head;
  DLLNode* tail;
  NodeEqFunc equalFunc;
} DLList;

typedef void(*NodeFunc)(DLLNode*);

