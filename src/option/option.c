#include <stdlib.h>
#include <stdbool.h>


enum State{
  OK,
  ERROR,
}

typedef struct{
  void* value;
  bool ok;
}Option;

#define Option()
