#include <cmath>
#include <algorithm>

// calculate image contrast
float contrast(const float bg, const float fg) {
  return std::abs(fg - bg)/std::max(bg, fg);
}

// calculate background for desired contrast
float findBg(const float fg, const float contrast) {
  return fg*(1.0 - contrast);
}
