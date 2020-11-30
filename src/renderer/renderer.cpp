#include "renderer.hpp"

#include <iostream>

Renderer::~Renderer() noexcept { glfwTerminate(); }

auto Renderer::Initialize() noexcept -> bool {
  // Initialise GLFW
  glewExperimental = 1;  // Needed for core profile
  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);                // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // To make MacOS happy
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE);  // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  window_ = gsl::owner<GLFWwindow*>{
      glfwCreateWindow(kWidth, kHeight, name_.c_str(), nullptr, nullptr)};
  if (window_ == nullptr) {
    std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they "
                 "are not 3.3 compatible. Try the 2.1 version of the tutorials."
              << std::endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window_);  // Initialize GLEW
  glewExperimental = 1;             // Needed in core profile
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

  return true;
}

auto Renderer::Run(GLuint program_id,
                   std::function<void()> const& draw_call) noexcept -> bool {
  // GLuint VertexArrayID{};
  // glGenVertexArrays(1, &VertexArrayID);
  // glBindVertexArray(VertexArrayID);

  // // An array of 3 vectors which represents 3 vertices
  // static const std::array<GLfloat, 9> g_vertex_buffer_data{
  //     -1.0F, -1.0F, 0.0F,  // v1
  //     +1.0F, -1.0F, 0.0F,  // v2
  //     +0.0F, +1.0F, 0.0F   // v3
  // };

  // // This will identify our vertex buffer
  // GLuint vertexbuffer{};
  // // Generate 1 buffer, put the resulting identifier in vertexbuffer
  // glGenBuffers(1, &vertexbuffer);
  // // The following commands will talk about our 'vertexbuffer' buffer
  // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // // Give our vertices to OpenGL.
  // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
  //              g_vertex_buffer_data.data(), GL_STATIC_DRAW);

  glClearColor(0.0F, 0.0F, 0.4F, 0.0F);  // NOLINT

  do {
    // Clear the screen. It's not mentioned before Tutorial 02, but it can
    // cause flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // NOLINT

    // Use our shader
    glUseProgram(program_id);

    // Draw
    draw_call();

    // Swap buffers
    glfwSwapBuffers(window_);
    glfwPollEvents();

  }  // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window_, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(window_) == 0);

  return true;
}