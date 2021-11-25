#include <algorithm>
#include <iostream>

#include "triangle.h"
#include "utils.h"

const int maxDistancePlaces = MAX_DISTANCE_PLACES;
const int maxDistancePower = pow(10.0, maxDistancePlaces);

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

  // construct OGL structures
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fVertices), fVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // determine max distance triangle can be displaced
  constexpr int perCoord = 2;
  constexpr int arrayLength = 3*perCoord;
  for (int i = 0; i < arrayLength; i+=perCoord) {
    float distance = sqrt(vertices[i]*vertices[i] + vertices[i+1]*vertices[i+1]);
    if (distance > fMaxDistance) {
      fMaxDistance = distance;
    }
  }
  fMaxDistanceMod = (float)(1.0 - floorTo(fMaxDistance, maxDistancePlaces)) * maxDistancePower;
  std::cout << "Max Distance: " << fMaxDistance << "\nMax distance mod: " << fMaxDistanceMod << std::endl;
}

void Triangle::GenerateDisplacements(float &outXDisp, float &outYDisp, float multiplier = 1.0) {
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
