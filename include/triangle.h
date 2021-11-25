#ifndef TRIANGLE_CLASS_H
#define TRIANGLE_CLASS_H 1

#ifndef MAX_DISTANCE_PLACES
#define MAX_DISTANCE_PLACES 5
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#include <glad/glad.h>

class Triangle {
private:
  float fCOM[2];
  float fVertices[9];
  float fMaxDistance;
  int fMaxDistanceMod;
  GLuint VBO;
  GLuint VAO;
public:
  Triangle(const float vertiecs[9]);
  ~Triangle() {};
  void GenerateDisplacements(float &outXDisp, float &outYDisp, float multiplier=1.0);
  void Draw();
};

#endif
