#include <algorithm>
#include <iostream>

#include "triangle.h"
#include "utils.h"

const int maxDistancePlaces = MAX_DISTANCE_PLACES;
const int maxDistancePower = pow(10.0, maxDistancePlaces);

// constexpr int perCoord = 2;
// constexpr int arrayLength = 3*perCoord;

Triangle::Triangle(const float vertices[6])
  : fMaxDistance(0), fMaxDistanceMod(0) {
  // copy vertices to internal
  std::copy(&vertices[0], &vertices[1]+1, &fVertices[0]);
  std::copy(&vertices[2], &vertices[3]+1, &fVertices[3]);
  std::copy(&vertices[4], &vertices[5]+1, &fVertices[6]);
  // set z to 0
  fVertices[2] = 0.0;
  fVertices[5] = 0.0;
  fVertices[8] = 0.0;

  // find COM of triangle
  fCOM[0] = 0.0;
  fCOM[1] = 0.0;
  for (int i = 0; i < 6; i += 2) {
    fCOM[0] += vertices[i];
    fCOM[1] += vertices[i+1];
  }
  fCOM[0] /= 3;
  fCOM[1] /= 3;

  // adjust vertices such that triangle is central
  // x coords
  fVertices[0] -= fCOM[0];
  fVertices[3] -= fCOM[0];
  fVertices[6] -= fCOM[0];
  // y coords
  fVertices[1] -= fCOM[1];
  fVertices[4] -= fCOM[1];
  fVertices[6] -= fCOM[1];

  // construct OGL structures
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fVertices), fVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // determine max distance triangle can be displaced
  for (int i = 0; i < 9; i+=3) {
    float distance = sqrt(fVertices[i]*fVertices[i] + fVertices[i+1]*fVertices[i+1]);
    if (distance > fMaxDistance) {
      fMaxDistance = distance;
    }
  }
  fMaxDistanceMod = (float)(1.0 - floorTo(fMaxDistance, maxDistancePlaces)) * maxDistancePower;
  std::cerr << "Max Distance: " << fMaxDistance << "\nMax distance mod: " << fMaxDistanceMod << std::endl;
}

void Triangle::GenerateDisplacements(float &outXDisp, float &outYDisp, float multiplier) {
  // generate a random float between 0 and fMaxDistance via fMaxDistanceMod
  float rDisp = (float)(rand() % fMaxDistanceMod) / maxDistancePower * multiplier;
  float thetaDisp = ((rand() % 36000) / 100.) * M_PI/180.;
  // convert polars to cartesian
  outXDisp = rDisp * cos(thetaDisp);
  outYDisp = rDisp * sin(thetaDisp);
}

void Triangle::Draw() {
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
