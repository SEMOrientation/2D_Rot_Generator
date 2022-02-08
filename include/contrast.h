#ifndef CONTRAST_H
#define CONTRAST_H 1

// calculate image contrast
float contrast(const float bg, const float fg);

// calculate background for desired contrast
float findBg(const float fg, const float contrast);

#endif
