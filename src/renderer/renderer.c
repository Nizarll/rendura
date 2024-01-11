#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdlib.h>
#include "../gl_object/gl_object.h"

void R_getViewMatrix(double* viewMatrix) {
    glGetDoublev(GL_MODELVIEW_MATRIX, viewMatrix);
}
void R_getInverseProjectionMatrix(double* inverseProjectionMatrix) {
    double op[16];
    glGetDoublev(GL_PROJECTION_MATRIX, op);

    double a = op[0];
    double b = op[5];
    double c = op[10];
    double d = op[14];
    double e = op[11];

    inverseProjectionMatrix[0] = 1.0 / a;
    inverseProjectionMatrix[5] = 1.0 / b;
    inverseProjectionMatrix[11] = 1.0 / d;
    inverseProjectionMatrix[14] = 1.0 / e;
    inverseProjectionMatrix[15] = -c / (d * e);
}

void multiplyMatrices(double* result, const double* matrix1, const double* matrix2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i * 4 + j] = 0.0;
            for (int k = 0; k < 4; k++) {
                result[i * 4 + j] += matrix1[i * 4 + k] * matrix2[k * 4 + j];
            }
        }
    }
}

Vector3 R_screenToWorldCoordinates(Vector2 screenSize, Vector2 mousePos) {
  double ndcX = (2.0 * mousePos.x / screenSize.x) - 1.0;
  double ndcY = 1.0 - (2.0 * mousePos.y / screenSize.y);
  double homogeneousCoord[4] = {ndcX, ndcY, 0.5, 1.0};

  double* inverseProjectionViewMatrix = malloc(sizeof(double[16]));
  double* viewMatrix = malloc(sizeof(double[16]));

  R_getViewMatrix(viewMatrix);
  R_getInverseProjectionMatrix(inverseProjectionViewMatrix);

  double viewProjectionMatrix[16];
  multiplyMatrices(viewProjectionMatrix, viewMatrix, inverseProjectionViewMatrix);

  double worldCoords[4] = {0.0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      worldCoords[i] += (float)viewProjectionMatrix[i * 4 + j] * homogeneousCoord[j];
    }
  }
  // Divide by w to get normalized device coordinates
  for (int i = 0; i < 3; i++) {
    worldCoords[i] /= worldCoords[3];
  }

  free(inverseProjectionViewMatrix);
  free(viewMatrix);

  return (Vector3){.x = worldCoords[0], .y = worldCoords[1], .z = worldCoords[2]};
}
