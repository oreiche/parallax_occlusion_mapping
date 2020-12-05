#ifndef WITH_OPENGL

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#else  // WITH_OPENGL

#define CATCH_CONFIG_RUNNER

#include "catch2/catch.hpp"
// include catch first
#include "GL/glew.h"
#include "GLFW/glfw3.h"

[[nodiscard]] static auto init_gl_context() noexcept -> bool {
  glewExperimental = GL_TRUE;
  if (glfwInit() == GLFW_TRUE) {
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);  // offscreen window
    auto window = glfwCreateWindow(640, 480, "", nullptr, nullptr);  // NOLINT
    if (window != nullptr) {
      glfwMakeContextCurrent(window);
      return (glewInit() == GLEW_OK);
    }
  }
  return false;
}

static void deinit_gl_context() noexcept { glfwTerminate(); }

auto main(int argc, char* argv[]) -> int {
  int result{-1};

  if (init_gl_context()) {
    result = Catch::Session().run(argc, argv);
  }

  deinit_gl_context();

  return result;
}

#endif  // WITH_OPENGL