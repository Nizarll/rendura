#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glew.h>
#include "../gl_object/gl_object.h"

double* R_getInverseProjectionMatrix();
void R_renderObject(Object *object);
Vector3 R_screenToWorldCoordinates(Vector2, Vector2); 

#endif
