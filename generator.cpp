#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image_write.h"

#include "shaderClass.h"

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

// uint8_t WINDOW_COLOR[4] = {0xdd, 0xcc, 0xff, 0xff};
const double INITIAL_WINDOW_COLOR[4] = {0x00/255., 0x00/255., 0x00/255., 0xff/255.};

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
  float vertices[] =
    {
      -0.5f, -0.33333333f, 0.0f,
      0.75f, -0.33333333f, 0.0f,
      -0.25f, 0.66666667f, 0.0f
    };
  GLuint VBO;
  glGenBuffers(1, &VBO);
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // build the shader program
  Shader simpleShader("./shaders/simpleVertShader.glsl", "./shaders/simpleFragShader.glsl");

  glClearColorArray(INITIAL_WINDOW_COLOR);

  constexpr float minrot = 0;
  constexpr float maxrot = 355;
  constexpr int numrots = 36*2;
  constexpr float step = (maxrot - minrot)/(numrots - 1);

  /* very basic render loop */
  while (!glfwWindowShouldClose(window)) {
    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  for (int i = 0; i < numrots; i++) {
    float angle = (minrot + step*i);

    // glClearColor(0xdd/255., 0xcc/255., 0xff/255., 0xff/255.);
    glClear(GL_COLOR_BUFFER_BIT);

    // use our shader to draw our vertices as a triangle
    simpleShader.use();
    // simpleShader.set3Vec("triColor", 0.85f, 0.85f, 0.85f);
    simpleShader.set3Vec("triColor", 1.0f, 1.0f, 1.0f);
    simpleShader.setFloat("theta", angle * M_PI/180.);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    char buffer[128];
    std::snprintf(buffer, 128, "%06.2f", angle);
    std::string fileName(buffer);
    fileName = fileName + ".png";
    save_image(window, fileName.c_str());
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
