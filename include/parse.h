#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  const char *str;
  size_t length;
} StringView;

typedef struct {
  const StringView **items;
  size_t count;
} Items;

char* readFile(char*);
size_t _countItems(char*, char);
Items splitBy(char*, char);
char* stringViewToString(const StringView*);

