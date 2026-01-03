#include "day8.h"
#include "util.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void day8(char *fileName) {
  day8Problem1(fileName);
  day8Problem2(fileName);
}


typedef struct {
  double x;
  double y;
  double z;
  int id;
} Point;


char* displayPoint(Point* point) {
  char* str = (char*)malloc(64);
  snprintf(
    str, 64,
    "Point(id=%d, x=%.2f, y=%.2f, z=%.2f)", point->id, point->x, point->y, point->z
  );
  return str;
}

Point* initPoint(const char* line, int id) {
    Items coords = splitBy((char*)line, ',');
    Point* point = (Point*)malloc(sizeof(Point));
    point->x = atof(stringViewToString(coords.items[0]));
    point->y = atof(stringViewToString(coords.items[1]));
    point->z = atof(stringViewToString(coords.items[2]));
    point->id = id;
    return point;
}

void _serialize_point(Point* item, char* buffer) {
  memcpy(buffer, item, sizeof(Point));
}

void _deserialize_point(char* buffer, Point* point) {
  memcpy(point, buffer, sizeof(Point));
}


typedef struct {
  Point* a;
  Point* b;
  double dist;
} PointDistance;

void _serialize_point_distance(PointDistance* item, char* buffer) {
  memcpy(buffer, item->a, sizeof(Point));
  memcpy(buffer + sizeof(Point), item->b, sizeof(Point));
  memcpy(buffer + 2*sizeof(Point), &(item->dist), sizeof(double));
}

void _deserialize_point_distance(char* buffer, PointDistance* pd) {
  Point* a = (Point*)malloc(sizeof(Point));
  memcpy(a, buffer, sizeof(Point));
  pd->a = a;
  Point* b = (Point*)malloc(sizeof(Point));
  memcpy(b, buffer + sizeof(Point), sizeof(Point));
  pd->b = b;
  pd->dist = *((double*)(buffer + 2*sizeof(Point)));
}

int _compare_point_distances(void* a, void* b) {
  PointDistance* pdA = (PointDistance*)a;
  PointDistance* pdB = (PointDistance*)b;
  if (pdA->dist < pdB->dist) {
    return -1;
  } else if (pdA->dist > pdB->dist) {
    return 1;
  } else {
    return 0;
  }
}

typedef struct {
  double* dists;
  size_t x;
  size_t y;
} DistanceMatrix;

DistanceMatrix* initDistanceMatrix(size_t x, size_t y) {
  DistanceMatrix* matrix = (DistanceMatrix*)malloc(sizeof(DistanceMatrix));
  matrix->dists = (double*)malloc(sizeof(double)*x*y);
  for (size_t i = 0; i < x*y; i++) {
    matrix->dists[i] = -1.0;
  }
  matrix->x = x;
  matrix->y = y;
  return matrix;
}

double lookupDistance(DistanceMatrix* matrix, size_t x, size_t y) {
  return matrix->dists[y*matrix->x + x];
}

void setDistance(DistanceMatrix* matrix, size_t x, size_t y, double dist) {
  matrix->dists[y*matrix->x + x] = dist;
}

typedef struct {
  Point** points;
  size_t length;
  size_t capacity;
} Circuit;

Circuit* initCircuit(size_t capacity) {
  Circuit* circuit = (Circuit*)malloc(sizeof(Circuit));
  circuit->points = (Point**)malloc(sizeof(Point)*capacity);
  circuit->length = 0;
  circuit->capacity = capacity;
  return circuit;
}

void addPointToCircuit(Circuit* circuit, Point* point) {
  if (circuit->length >= circuit->capacity) {
    circuit->capacity *= 2;
    circuit->points = (Point**)realloc(circuit->points, sizeof(Point*)*circuit->capacity);
  }
  circuit->points[circuit->length] = point;
  circuit->length += 1;
}

void combineCircuits(Circuit* a, Circuit* b) {
  for (size_t i = 0; i < b->length; i++) {
    addPointToCircuit(a, b->points[i]);
  }
}

void clearCircuit(Circuit* circuit) {
  circuit->length = 0;
}

typedef struct {
  Circuit** circuits;
  size_t length;
  size_t capacity;
} CircuitList;

CircuitList* initCircuitList(size_t capacity) {
  CircuitList* list = (CircuitList*)malloc(sizeof(CircuitList));
  list->circuits = (Circuit**)malloc(sizeof(Circuit)*capacity);
  list->length = 0;
  list->capacity = capacity;
  return list;
}

char* displayCircuit(Circuit* circuit) {
  char* str = (char*)malloc(1024);
  snprintf(str, 1024, "Circuit(length=%zu, points=[", circuit->length);
  for (size_t i = 0; i < circuit->length; i++) {
    char* pointStr = displayPoint(circuit->points[i]);
    strcat(str, pointStr);
    free(pointStr);
    if (i < circuit->length - 1) {
      strcat(str, ", ");
    }
  }
  strcat(str, "])");
  return str;
}

#define CIRCUIT_SHORT_STR_SIZE 32
char* displayCircuitShort(Circuit* circuit) {
  char* str = (char*)malloc(CIRCUIT_SHORT_STR_SIZE);
  snprintf(str, CIRCUIT_SHORT_STR_SIZE, "Circuit(length=%zu)", circuit->length);
  return str;
}

char* displayCircuitList(CircuitList* list) {
  char* str = (char*)malloc(list->length * CIRCUIT_SHORT_STR_SIZE +32);
  int written = snprintf(str, list->length * CIRCUIT_SHORT_STR_SIZE +32, "CircuitList(length=%zu, circuits=[", list->length);
  for (size_t i = 0; i < list->length; i++) {
    char* circuitStr = displayCircuitShort(list->circuits[i]);
    written += snprintf(str + written, list->length * CIRCUIT_SHORT_STR_SIZE +32 - written, "%s", circuitStr);
    free(circuitStr);
    if (i < list->length - 1) {
      written += snprintf(str + written, list->length * CIRCUIT_SHORT_STR_SIZE +32 - written, ", ");
    }
  }
  snprintf(str + written, 3, "])");
  return str;
}

int _compare_circuits(void* a, void* b) {
  Circuit* circuitA = (Circuit*)a;
  Circuit* circuitB = (Circuit*)b;
  return (int)(circuitA->length - circuitB->length);
}

void appendCircuitToList(CircuitList* list, Circuit* circuit) {
  if (list->length >= list->capacity) {
    list->capacity *= 2;
    list->circuits = (Circuit**)realloc(list->circuits, sizeof(Circuit)*list->capacity);
  }
  list->circuits[list->length] = circuit;
  list->length += 1;
}

Circuit* getCircuitByIndex(CircuitList* list, size_t index) {
  if (index >= list->length) {
    return NULL;
  }
  return list->circuits[index];
}

double distance(Point* a, Point* b) {
  return sqrt(pow(a->x-b->x, 2) + pow(a->y-b->y, 2) + pow(a->z-b->z, 2));
}

void serializePointDistances(PointDistance** distances, size_t distancesLen, char* cacheFileName) {
  int fh = open(cacheFileName, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
  size_t bufSize = sizeof(double) + 2*sizeof(Point);
  char buffer[bufSize];
  for (size_t i = 0; i < distancesLen; i++) {
    _serialize_point_distance(distances[i], buffer);
    write(fh, buffer, bufSize);
    memset(buffer, 0, bufSize);
  }
}

int deserializePointDistances(PointDistance** distances, size_t distancesLen, char* cacheFileName) {
  if (access(cacheFileName, F_OK) == -1) {
    printf("Cache file %s does not exist.\n", cacheFileName);
    return 0;
  }
  char* data = (char*)malloc(2 << 26); // ~128MB
  int fh = open(cacheFileName, O_RDONLY);
  read(fh, data, 2 << 26);
  size_t bufSize = sizeof(double) + 2*sizeof(Point);
  char buffer[bufSize];
  for (size_t i = 0; i < distancesLen; i++) {
    memcpy((void*)buffer, (void*)(data+(i*bufSize)), bufSize);
    PointDistance* pd = (PointDistance*)malloc(sizeof(PointDistance));
    _deserialize_point_distance(buffer, pd);
    distances[i] = pd;
    memset(buffer, 0, bufSize);
  }
  return 1;
}

#define TARGET_CONNECTIONS 1000
#define CACHE_FILE_NAME "day8_cache.bin"

int day8Problem1(char* fileName) {
  char* data = readFile(fileName);
  Items lines = splitBy(data, '\n');
  size_t pointCount = lines.count+1;
  Point** points = (Point**)malloc(sizeof(Point*)*pointCount);
  for (size_t i = 0; i < pointCount; i++) {
    const char* line = stringViewToString(lines.items[i]);
    points[i] = initPoint(line, i);
  }

  printf("Initialized %zu points.\n", pointCount);

  DistanceMatrix* matrix = initDistanceMatrix(pointCount, pointCount);
  CircuitList* circuits = initCircuitList(4);
  int* circuitIndex = (int*)malloc(sizeof(int)*pointCount);
  for (size_t i = 0; i < pointCount; i++) {
    circuitIndex[i] = -1;
  }

  printf("Calculating point distances...\n");
  PointDistance** pointDistances = (PointDistance**)malloc(sizeof(PointDistance*)*pointCount*pointCount);
  size_t offset = 0;
  size_t totalDistances = 0;
  for (size_t i = 0; i < pointCount; i++) {
    for (size_t j = 0; j < pointCount; j++) {
      if (points[i]->id == points[j]->id) {
        offset++;
        continue;
      }
      double dist = lookupDistance(matrix, i, j);
      if (dist != -1.0) {
        offset++;
        continue;
      }
      totalDistances++;
      dist = distance(points[i], points[j]);
      setDistance(matrix, i, j, dist);
      setDistance(matrix, j, i, dist);
      PointDistance* pd = (PointDistance*)malloc(sizeof(PointDistance));
      pd->a = points[i];
      pd->b = points[j];
      pd->dist = dist;
      pointDistances[i*pointCount + j - offset] = pd;
    }
  }

  printf("Sorting %zu point distances...\n", totalDistances);

  if(!deserializePointDistances(pointDistances, totalDistances, CACHE_FILE_NAME)) {
    quickSort(
      (void**)pointDistances,
      totalDistances,
      0,
      totalDistances,
      _compare_point_distances
    );
    printf("Serializing point distances to cache file %s...\n", CACHE_FILE_NAME);
    serializePointDistances(pointDistances, totalDistances, CACHE_FILE_NAME);
  } else {
    printf("Deserialized point distances from cache file %s.\n", CACHE_FILE_NAME);
  }

  printf("Connecting points into circuits...\n");
  int connectionsMade = 0;
  for (size_t i = 0; i < pointCount*pointCount-1; i++) {
    if (connectionsMade == TARGET_CONNECTIONS) {
      break;
    }
    PointDistance* pd = pointDistances[i];
    Point* a = pd->a;
    Point* b = pd->b;
    if (a->id == b->id) {
      continue;
    }
    if (circuitIndex[a->id] == -1 && circuitIndex[b->id] == -1) {
      Circuit* circuit = initCircuit(4);
      addPointToCircuit(circuit, a);
      addPointToCircuit(circuit, b);
      appendCircuitToList(circuits, circuit);
      circuitIndex[a->id] = circuits->length - 1;
      circuitIndex[b->id] = circuits->length - 1;
      connectionsMade++;
    } else if (circuitIndex[a->id] != -1 && circuitIndex[b->id] == -1) {
      Circuit* circuit = getCircuitByIndex(circuits, circuitIndex[a->id]);
      addPointToCircuit(circuit, b);
      circuitIndex[b->id] = circuitIndex[a->id];
      connectionsMade++;
    } else if (circuitIndex[a->id] == -1 && circuitIndex[b->id] != -1) {
      Circuit* circuit = getCircuitByIndex(circuits, circuitIndex[b->id]);
      addPointToCircuit(circuit, a);
      circuitIndex[a->id] = circuitIndex[b->id];
      connectionsMade++;
    } else if (circuitIndex[a->id] != -1 && circuitIndex[b->id] != -1 && circuitIndex[a->id] != circuitIndex[b->id]) {
      Circuit* circuitA = getCircuitByIndex(circuits, circuitIndex[a->id]);
      Circuit* circuitB = getCircuitByIndex(circuits, circuitIndex[b->id]);
      combineCircuits(circuitA, circuitB);
      for (size_t j = 0; j < circuitB->length; j++) {
        Point* pointInB = circuitB->points[j];
        circuitIndex[pointInB->id] = circuitIndex[a->id];
      }
      clearCircuit(circuitB);
      connectionsMade++;
    } else if(circuitIndex[a->id] == circuitIndex[b->id]) {
      connectionsMade++;
    }
  }

  for (size_t i = 0; i < pointCount; i++) {
    if (circuitIndex[i] == -1) {
      Circuit* circuit = initCircuit(1);
      addPointToCircuit(circuit, points[i]);
      appendCircuitToList(circuits, circuit);
    }
  }

  int trueCircuitCount = 0;
  for (size_t i = 0; i < circuits->length; i++) {
    Circuit* circuit = getCircuitByIndex(circuits, i);
    if (circuit->length > 0) {
      trueCircuitCount++;
    }
  }

  printf("Resulting circuits %d\n", trueCircuitCount);
  quickSort(
    (void**)circuits->circuits,
    circuits->length,
    0,
    circuits->length,
    _compare_circuits
  );


  long long total = 1;
  for (size_t i = circuits->length; i > 0; i--) {
    Circuit* circuit = getCircuitByIndex(circuits, i-1);
    if (i-1 < circuits->length-3) {
      continue;
    }
    total *= circuit->length;
  }

  printf("Day 8 Problem 1: %lld\n", total);

  return 0;
}

#define STARTING_CIRCUITS 1000

int day8Problem2(char* fileName) {
  char* data = readFile(fileName);
  Items lines = splitBy(data, '\n');
  size_t pointCount = lines.count+1;
  Point** points = (Point**)malloc(sizeof(Point*)*pointCount);
  for (size_t i = 0; i < pointCount; i++) {
    const char* line = stringViewToString(lines.items[i]);
    points[i] = initPoint(line, i);
  }

  printf("Initialized %zu points.\n", pointCount);

  DistanceMatrix* matrix = initDistanceMatrix(pointCount, pointCount);
  CircuitList* circuits = initCircuitList(4);
  int* circuitIndex = (int*)malloc(sizeof(int)*pointCount);
  for (size_t i = 0; i < pointCount; i++) {
    circuitIndex[i] = -1;
  }

  printf("Calculating point distances...\n");
  PointDistance** pointDistances = (PointDistance**)malloc(sizeof(PointDistance*)*pointCount*pointCount);
  size_t offset = 0;
  size_t totalDistances = 0;
  for (size_t i = 0; i < pointCount; i++) {
    for (size_t j = 0; j < pointCount; j++) {
      if (points[i]->id == points[j]->id) {
        offset++;
        continue;
      }
      double dist = lookupDistance(matrix, i, j);
      if (dist != -1.0) {
        offset++;
        continue;
      }
      totalDistances++;
      dist = distance(points[i], points[j]);
      setDistance(matrix, i, j, dist);
      setDistance(matrix, j, i, dist);
      PointDistance* pd = (PointDistance*)malloc(sizeof(PointDistance));
      pd->a = points[i];
      pd->b = points[j];
      pd->dist = dist;
      pointDistances[i*pointCount + j - offset] = pd;
    }
  }

  printf("Sorting %zu point distances...\n", totalDistances);

  if(!deserializePointDistances(pointDistances, totalDistances, CACHE_FILE_NAME)) {
    quickSort(
      (void**)pointDistances,
      totalDistances,
      0,
      totalDistances,
      _compare_point_distances
    );
    printf("Serializing point distances to cache file %s...\n", CACHE_FILE_NAME);
    serializePointDistances(pointDistances, totalDistances, CACHE_FILE_NAME);
  } else {
    printf("Deserialized point distances from cache file %s.\n", CACHE_FILE_NAME);
  }

  printf("Connecting points into circuits...\n");
  int circuitCount = STARTING_CIRCUITS;
  double total = 1.0;
  for (size_t i = 0; i < pointCount*pointCount-1; i++) {
    PointDistance* pd = pointDistances[i];
    Point* a = pd->a;
    Point* b = pd->b;
    if (a->id == b->id) {
      continue;
    }
    if (circuitIndex[a->id] == -1 && circuitIndex[b->id] == -1) {
      Circuit* circuit = initCircuit(4);
      addPointToCircuit(circuit, a);
      addPointToCircuit(circuit, b);
      appendCircuitToList(circuits, circuit);
      circuitIndex[a->id] = circuits->length - 1;
      circuitIndex[b->id] = circuits->length - 1;
      circuitCount--;
    } else if (circuitIndex[a->id] != -1 && circuitIndex[b->id] == -1) {
      Circuit* circuit = getCircuitByIndex(circuits, circuitIndex[a->id]);
      addPointToCircuit(circuit, b);
      circuitIndex[b->id] = circuitIndex[a->id];
      circuitCount--;
    } else if (circuitIndex[a->id] == -1 && circuitIndex[b->id] != -1) {
      Circuit* circuit = getCircuitByIndex(circuits, circuitIndex[b->id]);
      addPointToCircuit(circuit, a);
      circuitIndex[a->id] = circuitIndex[b->id];
      circuitCount--;
    } else if (circuitIndex[a->id] != -1 && circuitIndex[b->id] != -1 && circuitIndex[a->id] != circuitIndex[b->id]) {
      Circuit* circuitA = getCircuitByIndex(circuits, circuitIndex[a->id]);
      Circuit* circuitB = getCircuitByIndex(circuits, circuitIndex[b->id]);
      combineCircuits(circuitA, circuitB);
      for (size_t j = 0; j < circuitB->length; j++) {
        Point* pointInB = circuitB->points[j];
        circuitIndex[pointInB->id] = circuitIndex[a->id];
      }
      clearCircuit(circuitB);
      circuitCount--;
    }
    if (circuitCount == 1) {
      total *= a->x * b->x;
      break;
    }
  }

  printf("Day 8 Problem 2: %.2f", total);

  return 0;
}
