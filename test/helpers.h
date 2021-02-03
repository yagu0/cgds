#ifndef HELPERS_H
#define HELPERS_H

// types (POD) to be used as items inside our data structures
typedef struct {
  int a;
  double b;
} StructTest1;

typedef struct {
  float a;
  StructTest1* b;
} StructTest2;

#endif
