#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include <filesystem>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image_write.h"

#include "shaderClass.h"
#include "triangle.h"
#include "progressBar.h"
#include "contrast.h"
#include "utils.h"

constexpr int PROGRESS_BAR_SIZE = 30;

const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 512;

const std::filesystem::path TRAIN_DIR("train");
const std::filesystem::path TEST_DIR("test");

// uint8_t WINDOW_COLOR[4] = {0xdd, 0xcc, 0xff, 0xff};
const double INITIAL_WINDOW_COLOR[4] = {0x00/255., 0x00/255., 0x00/255., 0xff/255.};

// when true, rendering begins
bool shouldStartRendering = false;

// callback to resize the framebuffer if the user resizes the window
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// respond to input
void process_input(GLFWwindow *window) {
  // exit on ESC
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  // start rendering on SPACE
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    shouldStartRendering = true;
  }
}

void save_image(GLFWwindow *window, const char *fn) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  GLubyte *pixels = new GLubyte[3*width*height];
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  stbi_flip_vertically_on_write(true);
  stbi_write_png(fn, width, height, 3, pixels, 3*width);
}

#define glClearColorArray(color) \
  glClearColor(color[0], color[1], color[2], color[3])

#define glClearColorByteArray(color) \
  glClearColor(color[0] / 255., color[1] / 255., color[2] / 255., color[3] / 255.)

// generate xDisp and yDisp

int main() {
  /* initialise a GLFW window (LearnOpenGL 4) */
  glfwInit();

  // set required OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create the window
  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  /* initialise GLAD so we can access OpenGL function pointers (LearnOpenGL 4.1) */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialise GLAD!" << std::endl;
    glfwTerminate();
    return -1;
  }

  /* set viewport size */
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  /* register callback */
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /* construct vertices for a scalene triangle -- this has no
     rotational symmetry*/
  float vertices[6] =
    {
      -0.5f, -0.33333333f,
      0.75f, -0.33333333f,
      -0.25f, 0.66666667f
    };
  // construct Triangle object
  Triangle scalene(vertices);

  // build the shader program
  Shader simpleShader("./shaders/simpleVertShader.glsl", "./shaders/simpleFragShader.glsl");

  glClearColorArray(INITIAL_WINDOW_COLOR);

  constexpr float minrot = 0;
  constexpr float maxrot = 355;
  constexpr int numrots = 36*2;
  constexpr int numPerRot = 10;
  constexpr float step = (maxrot - minrot)/(numrots - 1);

  constexpr float maxContrast = 1;
  constexpr float minContrast = 0.9;
  constexpr float maxBrightness = 1;
  constexpr float minBrightness = 0.75;

  std::cerr << "creating output directories..." << std::endl;
  // try to create train and test directories
  if (std::filesystem::exists(TRAIN_DIR)) {
    if (!std::filesystem::is_empty(TRAIN_DIR)) {
      std::cerr << "cannot create training dir: exists and is not empty." << std::endl;
      return 0;
    }
  } else {
    if (!std::filesystem::create_directory(TRAIN_DIR)) {
      std::cerr << "failed to create training dir" << std::endl;
      return 0;
    }
  }
  if (std::filesystem::exists(TEST_DIR)) {
    if (!std::filesystem::is_empty(TEST_DIR)) {
      std::cerr << "cannot create test dir: exists and is not empty." << std::endl;
      return 0;
    }
  } else {
    if (!std::filesystem::create_directory(TEST_DIR)) {
      std::cerr << "failed to create test dir" << std::endl;
      return 0;
    }
  }  std::cerr << "done." << std::endl;
  
  // wait in a basic loop
  while (!glfwWindowShouldClose(window) && !shouldStartRendering) {
    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  if (glfwWindowShouldClose(window)) {
    return 0;
  }

  // generate training data
  std::cerr << "Generating training data...";
  ProgressBar trainBar(PROGRESS_BAR_SIZE, 0, numrots*numPerRot, true);
  for (int i = 0; i < numrots; ++i) {
    // check for premature exit
    if (glfwWindowShouldClose(window)) {
      return 0;
    }
    
    float angle = (minrot + step*i);

    // generate multiple images for each angle
    for (int j = 0; j < numPerRot; ++j) {
      // generate the displacements
      float xDisp;
      float yDisp;
      scalene.GenerateDisplacements(xDisp, yDisp);

      // generate a random brightness/contrast
      float brightness = randFloat(minBrightness, maxBrightness);
      float contrast = randFloat(minContrast, maxContrast);
      // set the background
      float bgShade = findBg(brightness, contrast);
      float bgColour[4] = {bgShade, bgShade, bgShade, 1.0};
      glClearColorArray(bgColour);

      glClear(GL_COLOR_BUFFER_BIT);

      // use our shader to draw our vertices as a triangle
      simpleShader.use();
      // simpleShader.set3Vec("triColor", 0.85f, 0.85f, 0.85f);
      simpleShader.set3Vec("triColor", brightness, brightness, brightness);
      simpleShader.setFloat("theta", angle * M_PI / 180.);
      simpleShader.setFloat("xDisp", xDisp);
      simpleShader.setFloat("yDisp", yDisp);
      scalene.Draw();

      char buffer[128];
      std::snprintf(buffer, 128, "%02d_%06.2f", j, angle);
      std::string fileName(buffer);
      fileName = fileName + ".png";
      save_image(window, (TRAIN_DIR / fileName).c_str());

      glfwSwapBuffers(window);
      glfwPollEvents();
      ++trainBar;
      trainBar.Display();
    }
  }
  std::cerr << std::endl;

  // generate test data
  constexpr int numtests = 5000;
  std::cerr << "Generating test data...";
  ProgressBar testBar(PROGRESS_BAR_SIZE, 0, numtests, true);
  for (int i = 0; i < numtests; ++i) {
    // check for premature exit
    if (glfwWindowShouldClose(window)) {
      return 0;
    }

    float angle = (rand() % 36000) / 100.;
    // generate the displacements
    float xDisp;
    float yDisp;
    scalene.GenerateDisplacements(xDisp, yDisp);

    // generate a random brightness/contrast
    float brightness = randFloat(minBrightness, maxBrightness);
    float contrast = randFloat(minContrast, maxContrast);
    // set the background
    float bgShade = findBg(brightness, contrast);
    float bgColour[4] = {bgShade, bgShade, bgShade, 1.0};
    glClearColorArray(bgColour);
    
    glClear(GL_COLOR_BUFFER_BIT);

    // use our shader to draw our vertices as a triangle
    simpleShader.use();
    // simpleShader.set3Vec("triColor", 0.85f, 0.85f, 0.85f);
    simpleShader.set3Vec("triColor", brightness, brightness, brightness);
    simpleShader.setFloat("theta", angle * M_PI/180.);
    simpleShader.setFloat("xDisp", xDisp);
    simpleShader.setFloat("yDisp", yDisp);
    scalene.Draw();

    char buffer[128];
    std::snprintf(buffer, 128, "%04d_%06.2f", i, angle);
    std::string fileName(buffer);
    fileName = fileName + ".png";
    save_image(window, (TEST_DIR / fileName).c_str());

    glfwSwapBuffers(window);
    glfwPollEvents();
    ++testBar;
    testBar.Display();
  }

  glfwTerminate();
  std::cerr << std::endl;
  return 0;
}
