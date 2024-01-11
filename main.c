#include <SDL2/SDL.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "src/gl_object/gl_object.h"
#include "src/renderer/renderer.h"

typedef struct Point3D {
  float x, y, z;
} Point3D;

int isInitialized = 0;
Vector3 *v = &(Vector3){.0f, .0f, .0f};
float lerp(float a, float b, float t){
  return a + t * (b - a);
}

Vector3 unprojectPoint(int x, int y) {
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLfloat winX, winY, winZ;
  GLdouble posX, posY, posZ;

  // Get the current viewport, modelview matrix, and projection matrix
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  // Invert the y-coordinate because OpenGL has (0,0) at the bottom left
  winX = (GLfloat)x;
  winY = (GLfloat)(viewport[3] - y);
  winZ = .98f;
  // Read the depth value from the depth buffer at the given screen coordinates
  // Unproject the screen coordinates to world coordinates
  gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
  return (Vector3){.x = (float)posX, .y = (float)posY, .z = (float)posZ};
}

Vertex frontFaceVertices[] = {
  // Front face
  { -1.0f, -1.0f,  1.0f },
  {  1.0f, -1.0f,  1.0f },
  {  1.0f,  1.0f,  1.0f },
  { -1.0f,  1.0f,  1.0f },
};

Vertex backFaceVertices[] = {
  // Back face
  { -1.0f, -1.0f, -1.0f },
  {  1.0f, -1.0f, -1.0f },
  {  1.0f,  1.0f, -1.0f },
  { -1.0f,  1.0f, -1.0f },
};

Point3D *cubeVertices[8];
Object *b;
Vector3 *center = &(Vector3){.x = 0, .y = 0, .z = 0};

void handleResize(int width, int height) {
  // Update the viewport
  glViewport(0, 0, width, height);
  // Update the perspective projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0f, (float)width / (float)height, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(3, 0, 5, 0, 0, 0, 0, 1, 0);
  // Update your object's center based on the new window size if needed
}

void initializeCube() {
  if (isInitialized) return;
  b = malloc(sizeof(struct Object));
  b->vertices = malloc(8 * sizeof(struct Vertex));
  b->center = &(Vector3){.0f, .0f, .0f};
  isInitialized = 1;
  for (int i = 0; i < 8; i++) {
    b->vertices[i] = i < 4 ? &frontFaceVertices[i] : &backFaceVertices[i - 4];
  }
}

float colors[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // color cache buffer for front vertices and back vertices
void renderCube(float angle) {
  initializeCube();
  angle = angle * 3.14 / 180;
  Vector3 v;
  v.x = 1;
  v.y = 1;
  v.z = 0;
  rotateXYZ((Object *)b, 8, (Vector3)v);
  /*for (int i = 0; i < 8; i++) {
    // Save the original values for reference
    float originalY = cubeVertices[i]->y;
    float originalZ = cubeVertices[i]->z;
    // Update y and z based on rotation angle [[Rz rotation matrix]]
    cubeVertices[i]->y = originalY * cos(angle) - originalZ * sin(angle);
    cubeVertices[i]->z = originalY * sin(angle) + originalZ * cos(angle);
    //LERP COLOR ON Z AXIS FOR SHADOW
    //---->>>>>
    colors[i] = lerp(colors[i],cubeVertices[i]->z > .5f ? 1.0f : .15f, .1);
    glColor3f(colors[i], colors[i], colors[i]);
    //
    glVertex3f(cubeVertices[i]->x, cubeVertices[i]->y, cubeVertices[i]->z);
    printf("x %lf y %lf z %lf\n", cubeVertices[i]->x, cubeVertices[i]->y, cubeVertices[i]->z);
  }
  glEnd();*/

}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }
  int width = 800, height = 600;
  SDL_Window* window = SDL_CreateWindow("OpenGL Cube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (context == NULL) {
    printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    return 1;
  }

  GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
    SDL_DestroyWindow(window);
    return 1;
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // Directional light from the top-right
  GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat specularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0f, (float)width/(float)height, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(3, 0, 5, 0, 0, 0, 0, 1, 0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  int quit = 0;
  SDL_Event e;
  float angle = 3.14/3;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }else if(e.type == SDL_MOUSEBUTTONDOWN){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        Vector3 pos = unprojectPoint(mouseX, mouseY);
        printf("%.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
        center = &(pos);
      }else if(e.type == SDL_KEYDOWN){
        switch(e.key.keysym.sym){
          case SDLK_z:
            printf("w pressed\n");
            break;
          case SDLK_a:
            printf("a pressed\n");
            break;
          case SDLK_d:
            printf("s pressed\n");
            break;
        }
      }else if(e.type == SDL_WINDOWEVENT){
        if(e.window.event == SDL_WINDOWEVENT_RESIZED){
          width = e.window.data1;
          height = e.window.data2;
          handleResize(width, height);
        }
      }
    }
    if(isInitialized){
      v->x = lerp(v->x, center->x, .1);
      v->y = lerp(v->y, center->y, .1); 
      v->z = lerp(v->z, center->z, .1);
      b->center = v;
      //b->center->z = lerp(b->center->z, center->z, .9);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderCube(angle);
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
