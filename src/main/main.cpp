#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "gsl/gsl-lite.hpp"
#include "src/shader_compiler/shader_compiler.hpp"

auto main() -> int {
  static constexpr auto kWidth = 1024;
  static constexpr auto kHeight = 768;

  // Initialise GLFW
  glewExperimental = 1;  // Needed for core profile
  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);                // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // To make MacOS happy
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE);  // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  GLFWwindow* window{nullptr};
  window = glfwCreateWindow(kWidth, kHeight, "Tutorial 01", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they "
                 "are not 3.3 compatible. Try the 2.1 version of the tutorials."
              << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);  // Initialize GLEW
  glewExperimental = 1;            // Needed in core profile
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  GLuint VertexArrayID{};
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // An array of 3 vectors which represents 3 vertices
  static const std::array<GLfloat, 9> g_vertex_buffer_data{
      -1.0F, -1.0F, 0.0F,  // v1
      +1.0F, -1.0F, 0.0F,  // v2
      +0.0F, +1.0F, 0.0F   // v3
  };

  // This will identify our vertex buffer
  GLuint vertexbuffer{};
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
               g_vertex_buffer_data.data(), GL_STATIC_DRAW);

  std::string vertex_shader_code =
      "#version 330 core\n \
layout(location = 0) in vec3 vertexPosition_modelspace; \
void main() { \
  gl_Position.xyz = vertexPosition_modelspace; \
  gl_Position.w = 1.0; \
}";

  std::string fragment_shader_code =
      "#version 330 core\n \
out vec3 color; \
void main() { \
  color = vec3(1, 0, 0); \
}";

  // Create and compile our GLSL program from the shaders
  auto program_id =
      ShaderCompiler{vertex_shader_code, fragment_shader_code}.Compile();

  glClearColor(0.0F, 0.0F, 0.4F, 0.0F);  // NOLINT
  do {
    // Clear the screen. It's not mentioned before Tutorial 02, but it can
    // cause flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // NOLINT

    // Use our shader
    glUseProgram(program_id);

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0,         // attribute 0. No particular reason for 0,
                                     // but must match the layout in the shader.
                          3,         // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          nullptr    // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0,
                 3);  // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  }  // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0);

  return 0;
}
