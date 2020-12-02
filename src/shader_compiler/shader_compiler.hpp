#ifndef SRC_SHADER_COMPILER_SHADER_COMPILER_HPP
#define SRC_SHADER_COMPILER_SHADER_COMPILER_HPP

#include <filesystem>
#include <optional>
#include <string>

#include "GL/glew.h"

class ShaderCompiler {
 public:
  ShaderCompiler(std::string vertex_shader,
                 std::string fragment_shader) noexcept
      : initialized_{true},
        vertex_shader_{std::move(vertex_shader)},
        fragment_shader_{std::move(fragment_shader)} {}

  ShaderCompiler(std::filesystem::path const& vertex_file_path,
                 std::filesystem::path const& fragment_file_path) noexcept {
    initialized_ = LoadShaderFromFile(vertex_file_path, fragment_file_path);
  }

  ShaderCompiler(ShaderCompiler const&) noexcept = delete;
  ShaderCompiler(ShaderCompiler&&) noexcept = delete;
  auto operator=(ShaderCompiler const&) noexcept -> ShaderCompiler& = delete;
  auto operator=(ShaderCompiler&&) noexcept -> ShaderCompiler& = delete;
  ~ShaderCompiler() noexcept = default;

  [[nodiscard]] auto Compile() && noexcept -> std::optional<GLuint>;
  [[nodiscard]] auto IsInitialized() const noexcept -> bool {
    return initialized_;
  }

 private:
  bool initialized_{};
  std::string vertex_shader_{};
  std::string fragment_shader_{};

  [[nodiscard]] auto LoadShaderFromFile(
      std::filesystem::path const& vertex_file_path,
      std::filesystem::path const& fragment_file_path) noexcept -> bool;
};

#endif  // SRC_SHADER_COMPILER_SHADER_COMPILER_HPP