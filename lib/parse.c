#include "../include/parse.h"

char* readFile(char *path) {
  int fh = open(path, O_RDONLY);

  if (fh == -1) {
    char buf[1024];
    sprintf(buf, "Failed to open file: %s\n", path);
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
    sprintf(buf, "Failed to read file: %s\n", path);
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
      continue;
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
