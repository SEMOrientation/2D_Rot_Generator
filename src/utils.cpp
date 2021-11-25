#include <cmath>
#include "utils.h"

// round to a certain number of decimal places
float floorTo(float value, int places) {
  float power = pow(10.0, places);
  return floor(value*power)/power;
}
