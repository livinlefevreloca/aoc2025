#include "../include/dll.h"

DLLNode* dllNodeInit(void* value, DLLNode* prev) {
  DLLNode* node = (DLLNode*)malloc(sizeof(DLLNode));
  node->value =value;
  node->prev = prev;
  return node;
}

void dllNodeFree(DLLNode* node) {
  free((void*)node);
}

DLList dllInitFromArr(void** items, size_t size, size_t length, NodeEqFunc func) {
  DLList list;
  DLLNode* head = dllNodeInit(items[0], NULL);
  list.head = head;
  list.tail = head;
  list.equalFunc = func;
  for (size_t i = size; i < length; i+= size) {
    DLLNode* next = dllNodeInit(items[i], list.tail);
    next->prev = list.tail;
    list.tail->next = next;
    list.tail = next;
  }

  return list;
}

DLList dllInit(void* value, NodeEqFunc func) {
  DLLNode* head = dllNodeInit(value, NULL);
  DLList list;
  list.head = head;
  list.tail = head;
  list.equalFunc = func;

  return list;
}

void dllForEach(DLList* list, NodeFunc func) {
  DLLNode* head = list->head;
  while (head != NULL) {
    func(head);
    head = head->next;
  }
}

void dllForEachBack(DLList* list, NodeFunc func) {
  DLLNode* tail = list->tail;
  while (tail != NULL) {
    func(tail);
    tail = tail->prev;
  }
}

DLLNode* dllFindNode(DLList* list, void* other) {
  DLLNode* head = list->head;
  while (head != NULL) {
    DLLNode* result = list->equalFunc(head, other);
    if (result != NULL) {
      return result;
    }
    head = head->next;
  }
  return NULL;
}

DLLNode* dllFindNodeBack(DLList* list, void* other) {
  DLLNode* tail = list->tail;
  while (tail != NULL) {
    DLLNode* result = list->equalFunc(tail, other);
    if (result != NULL) {
      return result;
    }
    tail = tail->prev;
  }
  return NULL;
}

void dllAppend(DLList* list, void* value) {
  DLLNode* node = dllNodeInit(value, list->tail);
  list->head->next = node;
}

void dllPrepend(DLList* list, void* value) {
  DLLNode* node = dllNodeInit(value, NULL);
  list->head->prev = node;
  node->next = list->head;
  list->head = node;
}

void freeDLList(DLList* list) {
  // Free each DLLNode
  dllForEach(list, dllNodeFree);
  free((void*)list);
}
