#include "gl_object.h"
#include <math.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

Vector3 get2DFrom3DPos(Vector2 v){
  double x, y, z;
  double model_view[16];
  double projection[16];
  int viewport[4];
  glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);
  gluUnProject(v.x, viewport[3] - v.y, 0.0, model_view, projection, viewport, &x, &y, &z);
  return (Vector3){ .x = (float)x, .y = (float)y, .z = (float)z};
}

void translateXYZ(Object *object, int len, Vector3 position){
  object->center = &(Vector3){(float)position.x, (float)position.y, (float)position.z};
}

void rotateXYZ(Object *object, int len, Vector3 rotation) {
  float x, y, z;
  rotation.x *= 3.14/180, rotation.y *= 3.14 / 180, rotation.z *= 3.14/180;
  float cosX = cos(rotation.x);
  float sinX = sin(rotation.x);
  float cosY = cos(rotation.y);
  float sinY = sin(rotation.y);
  float cosZ = cos(rotation.z);
  float sinZ = sin(rotation.z);
  glBegin(GL_LINES);
  for(int i = 0; i < len; i++) {
    x = object->vertices[i]->x;
    y = object->vertices[i]->y;
    z = object->vertices[i]->z;
    // Apply Rx, Ry, and Rz rotations
    object->vertices[i]->x = x * cosY - y * cosY * sinZ + z * sinY;
    object->vertices[i]->y = x * ( cosX * sinZ + sinX * sinY * cosZ ) + y * ( cosX * cosZ - sinX * sinX * sinY * sinZ) - z * sinX * cosY;
    object->vertices[i]->z = x * ( sinX * sinZ - cosX * sinY * cosZ ) + y * ( sinX * cosZ + cosX * sinY * sinZ) + z * cosX * cosY; 
    float color3 = object->vertices[i]->z < .2f ? 1.0f : .6f;
    for(int j = i + 1; j < len; j++){
      glVertex3f(object->vertices[i]->x + object->center->x,object->vertices[i]->y + object->center->y,object->vertices[i]->z + object->center->z);
      glVertex3f(object->vertices[j]->x + object->center->x,object->vertices[j]->y + object->center->y,object->vertices[j]->z + object->center->z);
    }
  }
  glEnd();
}
