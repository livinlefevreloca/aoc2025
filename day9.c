#include "include/day9.h"
#include "include/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void day9(char *fileName) {
  day9Problem1(fileName);
  day9Problem2(fileName);
}

typedef enum {
  X,
  Y,
} Direction;

typedef struct {
  long long x;
  long long y;
  int id;
} Point2D;

typedef struct {
  Point2D* a;
  Point2D* b;
  long long area;
} PointArea2D;

typedef struct {
  Point2D* a;
  Point2D* b;
  Direction direction;
  int id;
} Line;


Point2D* initPoint2D(char* line, int id);
PointArea2D* calcPointArea2D(Point2D* a, Point2D* b);
int taxicabDistance(Point2D* p1, Point2D* p2);
char* displayPoint2D(Point2D* point);
char* displayPointArea2D(PointArea2D* pd);
long long calcRectangleArea(Point2D* p1, Point2D* p2);
int linesIntersect(Line* l1, Line* l2);
Line* initLine(Point2D*, Point2D*, int);
Point2D* coordsToPoint2D(long long x, long long y);
void freeLine(Line* line);
void freePoint2D(Point2D* point);
void freePointArea2D(PointArea2D* pd);


Point2D* initPoint2D(char* line, int id) {
  Items coords = splitBy(line, ',');
  Point2D* point = (Point2D*)malloc(sizeof(Point2D));

  point->x = atol(coords.items[0]->str);
  point->y = atol(coords.items[1]->str);
  point->id = id;

  return point;
}

void freePoint2D(Point2D* point) {
  free(point);
}

Point2D* coordsToPoint2D(long long x, long long y) {
  Point2D* point = (Point2D*)malloc(sizeof(Point2D));
  point->x = x;
  point->y = y;
  point->id = -1;

  return point;
}

PointArea2D* calcPointArea2D(Point2D* a, Point2D* b) {
  PointArea2D* pd = (PointArea2D*)malloc(sizeof(PointArea2D));
  pd->a = a;
  pd->b = b;
  pd->area = calcRectangleArea(a, b);
  return pd;
}

void freePointArea2D(PointArea2D* pd) {
  freePoint2D(pd->a);
  freePoint2D(pd->b);
  free(pd);
}

int isEqualPoint2D(Point2D* a, Point2D* b) {
  return a->x == b->x && a->y == b->y;
}

Line** getPerimeterFromArea(PointArea2D* area) {
  Line** lines = (Line**)malloc(sizeof(Line*)*4);
  lines[0] = initLine(coordsToPoint2D(area->a->x, area->a->y), coordsToPoint2D(area->b->x, area->a->y), X);
  lines[1] = initLine(coordsToPoint2D(area->b->x, area->a->y), coordsToPoint2D(area->b->x, area->b->y), Y);
  lines[2] = initLine(coordsToPoint2D(area->b->x, area->b->y), coordsToPoint2D(area->a->x, area->b->y), X);
  lines[3] = initLine(coordsToPoint2D(area->a->x, area->b->y), coordsToPoint2D(area->a->x, area->a->y), Y);
  return lines;
}

char* displayPoint2D(Point2D* point) {
  char* str = (char*)malloc(64);
  snprintf(str, 64, "Point2D(id=%d, x=%lld, y=%lld)", point->id, point->x, point->y);
  return str;
}

char* displayPointArea2D(PointArea2D* pd) {
  char* str = (char*)malloc(128);
  char* aStr = displayPoint2D(pd->a);
  char* bStr = displayPoint2D(pd->b);
  snprintf(str, 128, "PointArea2D(a=%s, b=%s, area=%lld)", aStr, bStr, pd->area);
  free(aStr);
  free(bStr);
  return str;
}

long long calcRectangleArea(Point2D* p1, Point2D* p2) {
  long long width = llabs(p1->x - p2->x)+1;
  long long height = llabs(p1->y - p2->y)+1;
  return width * height;
}

Line* initLine(Point2D* a, Point2D* b, int id) {
  Line* line = (Line*)malloc(sizeof(Line));
  line->a = a;
  line->b = b;
  line->id = id;
  if (b->x == a->x) {
    line->direction = Y;
  } else {
    line->direction = X;
  }
  return line;
}

void freeLine(Line* line) {
  freePoint2D(line->a);
  freePoint2D(line->b);
  free(line);
}

Line* shiftLineOut(Line* prev, Line* l, Line* after) {
  Line* newLine = initLine(coordsToPoint2D(l->a->x, l->a->y), coordsToPoint2D(l->b->x, l->b->y), l->id);
  if (newLine->direction == X) {
    int lineLeft = newLine->a->x > newLine->b->x;
    int prevLineUp = (prev->a->y > prev->b->y) << 1;
    int nextLineUp = (after->a->y > after->b->y) << 2;
    int dirFlags = nextLineUp | prevLineUp | lineLeft;
    switch (dirFlags) {
      /*
       |
       |
       V #------#
       x------>x
               |
               |
               V
      */
      case 0: // lineRight, prevLineDown, nextLineDown
        newLine->a->y-=1;
        newLine->a->x+=1;
        newLine->b->y-=1;
        newLine->b->x+=1;
        break;
      /*
                 |
                 |
                 V
         x<------x
         | #------#
         |
         V
      */
      case 1: // lineLeft, prevLineDown, nextLineDown
        newLine->a->y+=1;
        newLine->a->x+=1;
        newLine->b->y+=1;
        newLine->b->x+=1;
        break;
      /*

       #---------#
        x------>x
        ^       |
        |       |
        |       V
      */
      case 2: // lineRight, prevLineUp, nextLineDown
        newLine->a->y-=1;
        newLine->a->x-=1;
        newLine->b->y-=1;
        newLine->b->x+=1;
        break;
      /*
        x<------x
        | #---# ^
        |       |
        V       |
      */
      case 3: // lineLeft, prevLineUp, nextLineDown
        newLine->a->y+=1;
        newLine->a->x-=1;
        newLine->b->y+=1;
        newLine->b->x+=1;
        break;
      /*

        |       ^
        |       |
        V #---# |
        x------>x
      */
      case 4: // lineRight, prevLineDown, nextLineUp
        newLine->a->y-=1;
        newLine->a->x+=1;
        newLine->b->y-=1;
        newLine->b->x-=1;
        break;
      /*
        ^       |
        |       |
        |       V
        x<------x
       #---------#
      */
      case 5: // lineLeft, prevLineDown, nextLineUp
        newLine->a->y+=1;
        newLine->a->x+=1;
        newLine->b->y+=1;
        newLine->b->x-=1;
        break;
      /*
               ^
               |
      #------# |
       x------>x
       ^
       |
       |
      */
      case 6: // lineRight, prevLineUp, nextLineUp
        newLine->a->y-=1;
        newLine->a->x-=1;
        newLine->b->y-=1;
        newLine->b->x-=1;
        break;

      /*
       ^
       |
       |
       x<------x
      #------# ^
               |
               |
      */
      case 7: // lineLeft, prevLineUp, nextLineUp
        newLine->a->y+=1;
        newLine->a->x-=1;
        newLine->b->y+=1;
        newLine->b->x-=1;
        break;
      default:
        panic("Unknown line direction combination");
    }
  } else if (l->direction == Y) {
    int lineUp = newLine->a->y > newLine->b->y;
    int prevLineRight = (prev->a->x < prev->b->x) << 1;
    int nextLineRight = (after->a->x < after->b->x) << 2;
    int dirFlags = nextLineRight | prevLineRight | lineUp;
    switch (dirFlags) {
      /*        #
        ----->x |
              | |
              | |
              V #
              x----->
      */
      case 0: // lineDown, prevLineLeft, nextLineLeft
        newLine->a->x+=1;
        newLine->a->y-=1;
        newLine->b->x+=1;
        newLine->b->y-=1;
        break;
      /*

      <-----x
          # ^
          | |
          | |
          | x<-----
          #
      */
      case 1: // lineUp, prevLineLeft, nextLineLeft
        newLine->a->x-=1;
        newLine->a->y+=1;
        newLine->b->x-=1;
        newLine->b->y+=1;
        break;
      /*
                #
       ------>x |
              | |
              | |
              V |
       <------x |
                #
      */
      case 2: // lineDown, prevLineRight, nextLineLeft
        newLine->a->x+=1;
        newLine->a->y-=1;
        newLine->b->x+=1;
        newLine->b->y+=1;
        break;
      /*
       <------x
            # ^
            | |
            # |
       ------>x
      */
      case 3: // lineUp, prevLineRight, nextLineLeft
        newLine->a->x-=1;
        newLine->a->y-=1;
        newLine->b->x-=1;
        newLine->b->y+=1;
        break;
      /*
        x<-------
        | #
        | |
        V #
        x------->

      */
      case 4: // lineDown, prevLineLeft, nextLineRight
        newLine->a->x+=1;
        newLine->a->y+=1;
        newLine->b->x+=1;
        newLine->b->y-=1;
        break;
      /*
       #
       |x------>
       |^
       ||
       ||
       |x<------
       #
      */
      case 5: // lineUp, prevLineLeft, nextLineRight
        newLine->a->x-=1;
        newLine->a->y+=1;
        newLine->b->x-=1;
        newLine->b->y-=1;
        break;
      /*
                 #
       ------->x |
               | |
               | |
               V #
               x------->
      */
      case 6: // lineDown, prevLineRight, nextLineRight
        newLine->a->x+=1;
        newLine->a->y-=1;
        newLine->b->x+=1;
        newLine->b->y-=1;
        break;
      /*    #
            | x------->
            | ^
            | |
            # |
       ------>x
      */
      case 7: // lineUp, prevLineRight, nextLineRight
        newLine->a->x-=1;
        newLine->a->y-=1;
        newLine->b->x-=1;
        newLine->b->y-=1;
        break;
      default:
        panic("Unknown line direction combination");
    }
  } else {
    panic("Unknown line direction in shiftLineOut");
  }
  return newLine;
}

int linesIntersect(Line* l1, Line* l2) {
  if (l1->direction == l2->direction) {
    if (l1->direction == X){
      return l1->a->y == l2->b->y;
    } else if (l1->direction == Y) {
      return l1->a->x == l2->b->x;
    } else {
      panic("Unkown direction for same line");
    }

  }

  Line* hLine = l1;
  Line* vLine = l2;
  if (l1->direction == X) {
    hLine = l1;
    vLine = l2;
  } else if (l1->direction == Y) {
    hLine = l2;
    vLine = l1;
  } else {
    panic("Unknown Direction in intersection");
  }
  long long hY = hLine->a->y;
  long long vX = vLine->a->x;

  long long yMin = MIN(vLine->a->y, vLine->b->y);
  long long yMax = MAX(vLine->a->y, vLine->b->y);
  long long xMin = MIN(hLine->a->x, hLine->b->x);
  long long xMax = MAX(hLine->a->x, hLine->b->x);
  return (hY <= yMax && hY >= yMin) && (vX <= xMax && vX >= xMin);
}


int day9Problem1(char* fileName) {
  char* data = readFile(fileName);
  Items lines = splitBy(data, '\n');
  size_t pointCount = lines.count;

  Point2D** points = (Point2D**)malloc(pointCount*sizeof(Point2D*));
  for (size_t i = 0; i <= pointCount; i++) {
    const StringView* line = lines.items[i];
    Point2D* point = initPoint2D((char*)line->str, i);
    points[i] = point;
  }

  PointArea2D** areas = (PointArea2D**)malloc(pointCount * pointCount * sizeof(PointArea2D*));
  for (size_t i = 0; i < pointCount; i++) {
    for (size_t j = 0; j < pointCount; j++) {
      PointArea2D* pd = calcPointArea2D(points[i], points[j]);
      areas[i * pointCount + j] = pd;
    }
  }

  PointArea2D* maxArea = NULL;
  for (size_t i = 0; i < pointCount * pointCount; i++) {
    if (maxArea == NULL || areas[i]->area > maxArea->area) {
      maxArea = areas[i];
    }
  }

  printf("Problem 1 Day 9: %lld\n", maxArea->area);
  return 0;
}

PointArea2D* getNextLargestArea(PointArea2D** areas, size_t areaCount) {
  PointArea2D* maxArea = NULL;
  for (size_t i = 0; i < areaCount; i++) {
    if (maxArea == NULL || areas[i]->area > maxArea->area) {
      maxArea = areas[i];
    }
  }
  return maxArea;
}

int day9Problem2(char* fileName) {
  char* data = readFile(fileName);
  Items fileLines = splitBy(data, '\n');
  size_t pointCount = fileLines.count+1;

  Point2D** points = (Point2D**)malloc(pointCount*sizeof(Point2D*));
  size_t sideCount = pointCount;
  Line** shape = (Line**)malloc(sideCount*sizeof(Line*));
  Line** lines = (Line**)malloc((sideCount)*sizeof(Line*));
  for (size_t i = 0; i < pointCount; i++) {
    const StringView* fileLine = fileLines.items[i];
    Point2D* point = initPoint2D((char*)fileLine->str, i);
    points[i] = point;

    if (i == 0) {
      continue;
    } else if (i == pointCount-1) {
      Line* line = initLine(points[i], points[0], i);
      lines[0] = line;
    }
    Line* line = initLine(points[i-1], points[i], i);
    lines[i] = line;
  }
  // Shift lines outwards to create the shape the surrounds the points.
  // we need to do this so we can do a simple check for intersections later.
  for (size_t i = 0; i < sideCount; i++) {
    Line* prev = lines[(i + sideCount -1) % sideCount];
    Line* curr = lines[i];
    Line* after = lines[(i + 1) % sideCount];
    Line* shifted = shiftLineOut(prev, curr, after);
    shape[i] = shifted;
  }

  PointArea2D** areas = (PointArea2D**)malloc(pointCount * pointCount * sizeof(PointArea2D*));
  int** matrix = (int**)malloc(pointCount * sizeof(int*));
  for (size_t i = 0; i < pointCount; i++) {
    matrix[i] = (int*)malloc(pointCount * sizeof(int));
    memset(matrix[i], 0, pointCount * sizeof(int));
  }
  size_t offset = 0;
  for (size_t i = 0; i < pointCount; i++) {
    for (size_t j = 0; j < pointCount; j++) {
      if (i == j) {
        offset++;
        continue;
      }
      if (matrix[i][j] == 1) {
        offset++;
        continue;
      }
      PointArea2D* pd = calcPointArea2D(points[i], points[j]);
      areas[(i * pointCount + j) - offset] = pd;
      matrix[i][j] = 1;
      matrix[j][i] = 1;
    }
  }

  size_t startingIdx = pointCount*pointCount-offset;
  for (size_t i = startingIdx; i > 0; i--) {
    // We could make this more efficient by sorting once and iterating,
    // but my quicksort implementation doesnnt seem to work
    // properly here.
    PointArea2D* area = getNextLargestArea(areas, startingIdx);
    Line** perim = getPerimeterFromArea(area);
    int foundIntersection = 0;
    for (size_t j = 0; j < 4; j++) {
      for (size_t k = 0; k < sideCount; k++) {
        if (linesIntersect(perim[j], shape[k])) {
          foundIntersection = 1;
          break;
        }
      }
      if (foundIntersection) {
        break;
      }
    }
    if (!foundIntersection) {
      printf("Problem 2 Day 9: %lld\n", area->area);
      break;
    }
    area->area=0;
  }
  return 0;
}
