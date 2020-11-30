#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "gsl/gsl-lite.hpp"

auto LoadShaders(std::string const& VertexShaderCode,
                 std::string const& FragmentShaderCode) noexcept -> GLuint;

auto LoadShadersFromFile(
    std::filesystem::path const& vertex_file_path,
    std::filesystem::path const& fragment_file_path) noexcept -> GLuint;

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
  GLuint programID = LoadShaders(vertex_shader_code, fragment_shader_code);

  glClearColor(0.0F, 0.0F, 0.4F, 0.0F);  // NOLINT
  do {
    // Clear the screen. It's not mentioned before Tutorial 02, but it can
    // cause flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // NOLINT

    // Use our shader
    glUseProgram(programID);

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

auto LoadShaders(std::string const& VertexShaderCode,
                 std::string const& FragmentShaderCode) noexcept -> GLuint {
  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  GLint Result = GL_FALSE;
  int InfoLogLength{};

  // Compile Vertex Shader
  std::cout << "Compiling vertex shader" << std::endl;
  char const* VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> VertexShaderErrorMessage(
        gsl::narrow<std::size_t>(InfoLogLength + 1));
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr,
                       &VertexShaderErrorMessage[0]);
    std::cout << &VertexShaderErrorMessage[0] << std::endl;
  }

  // Compile Fragment Shader
  std::cout << "Compiling fragment shader" << std::endl;
  char const* FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> FragmentShaderErrorMessage(
        gsl::narrow<std::size_t>(InfoLogLength + 1));
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr,
                       &FragmentShaderErrorMessage[0]);
    std::cout << &FragmentShaderErrorMessage[0] << std::endl;
  }

  // Link the program
  std::cout << "Linking program" << std::endl;
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> ProgramErrorMessage(
        gsl::narrow<std::size_t>(InfoLogLength + 1));
    glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr,
                        &ProgramErrorMessage[0]);
    std::cout << &ProgramErrorMessage[0] << std::endl;
  }

  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

auto LoadShadersFromFile(
    std::filesystem::path const& vertex_file_path,
    std::filesystem::path const& fragment_file_path) noexcept -> GLuint {
  try {
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
      std::stringstream sstr;
      sstr << VertexShaderStream.rdbuf();
      VertexShaderCode = sstr.str();
      VertexShaderStream.close();
    } else {
      std::cout << "Impossible to open" << vertex_file_path << ". "
                << "Are you in the right directory? "
                << "Don't forget to read the FAQ !" << std::endl;
      getchar();
      return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
      std::stringstream sstr;
      sstr << FragmentShaderStream.rdbuf();
      FragmentShaderCode = sstr.str();
      FragmentShaderStream.close();
    }

    return LoadShaders(VertexShaderCode, FragmentShaderCode);
  } catch (...) {
    std::exit(EXIT_FAILURE);
  }
}