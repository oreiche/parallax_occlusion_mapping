#ifndef SRC_RENDERER_RENDERER_COMPILER_HPP
#define SRC_RENDERER_RENDERER_COMPILER_HPP

#include <functional>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "gsl/gsl-lite.hpp"

class Renderer {
 public:
  explicit Renderer(std::string name) noexcept : name_{std::move(name)} {
    if (!Initialize()) {
      std::exit(EXIT_FAILURE);
    }
  }
  Renderer(Renderer const&) noexcept = delete;
  Renderer(Renderer&&) noexcept = delete;
  ~Renderer() noexcept;
  auto operator=(Renderer const&) noexcept -> Renderer& = delete;
  auto operator=(Renderer&&) noexcept -> Renderer& = default;

  [[nodiscard]] auto Run(GLuint program_id,
                         std::function<void()> const& draw_call) noexcept
      -> bool;

 private:
  static constexpr auto kWidth = 1024;
  static constexpr auto kHeight = 768;
  std::string name_;
  gsl::owner<GLFWwindow*> window_{};

  [[nodiscard]] auto Initialize() noexcept -> bool;
};

#endif  // SRC_RENDERER_RENDERER_COMPILER_HPP