#ifndef SRC_POM_RENDERER_RENDERER_HPP
#define SRC_POM_RENDERER_RENDERER_HPP

#include <functional>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "gsl/gsl-lite.hpp"
#include "src/pom/renderer/camera.hpp"

class Renderer {
 public:
  using DrawCallback = std::function<bool()>;

  explicit Renderer(std::string name) noexcept : name_{std::move(name)} {
    initialized_ = Initialize();
  }
  Renderer(Renderer const&) noexcept = delete;
  Renderer(Renderer&&) noexcept = delete;
  ~Renderer() noexcept;
  auto operator=(Renderer const&) noexcept -> Renderer& = delete;
  auto operator=(Renderer&&) noexcept -> Renderer& = default;

  void SetExitKeyCode(int keycode) noexcept { exit_keycode_ = keycode; }

  [[nodiscard]] auto Run(DrawCallback const& draw_call) noexcept -> bool;

  [[nodiscard]] auto GetViewMatrix() const noexcept -> glm::mat4 {
    return camera_.GetViewMatrix();
  }

 private:
  static constexpr auto kWidth = 1024;
  static constexpr auto kHeight = 768;
  bool initialized_{};
  std::string name_{};
  GLFWwindow* window_{};
  Camera camera_{};
  int exit_keycode_{GLFW_KEY_ESCAPE};

  [[nodiscard]] auto Initialize() noexcept -> bool;
  [[nodiscard]] auto HandleKey() noexcept -> bool;
};

#endif  // SRC_POM_RENDERER_RENDERER_HPP