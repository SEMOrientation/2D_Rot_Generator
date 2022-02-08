#include <cmath>
#include "utils.h"

// round to a certain number of decimal places
float floorTo(float value, int places) {
  float power = pow(10.0, places);
  return floor(value*power)/power;
}

// get a random float between 0 and 1
// generally easier to work with than rand() on its own
float randFloat() {
  return (rand() % 100000000) / 100000000.0;
}

// get a random float in a particular range
float randFloat(const float min, const float max) {
  return min + randFloat()*(max - min);
}
