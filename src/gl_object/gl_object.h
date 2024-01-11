#ifndef GL_OBJECT
#define GL_OBJECT
#include <stdint.h>

typedef struct Vector3{
  float x, y, z;
}Vector3;

typedef struct Vector2{
  float x, y;
}Vector2;

typedef struct Vertex{
  float x, y, z;
}Vertex;

typedef struct Object{
  Vertex **vertices;
  Vector3 *center;
}Object;

Vector3 get2DFrom3DPos(Vector2);
void translateXYZ(Object*, int, Vector3);
void rotateXYZ(Object*, int, Vector3);
void scaleXYZ(Object*, Vector3);
void moveTo(Object*, Vector3);
void initialize(Object);

#endif
