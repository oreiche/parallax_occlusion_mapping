#ifndef SRC_SHADER_COMPILER_SHADER_COMPILER_HPP
#define SRC_SHADER_COMPILER_SHADER_COMPILER_HPP

#include <filesystem>
#include <string>

#include "GL/glew.h"

class ShaderCompiler {
 public:
  ShaderCompiler(std::string vertex_shader,
                 std::string fragment_shader) noexcept
      : vertex_shader_{std::move(vertex_shader)},
        fragment_shader_{std::move(fragment_shader)} {}

  ShaderCompiler(std::filesystem::path const& vertex_file_path,
                 std::filesystem::path const& fragment_file_path) noexcept {
    if (!LoadShaderFromFile(vertex_file_path, fragment_file_path)) {
      std::exit(EXIT_FAILURE);
    }
  }

  ShaderCompiler(ShaderCompiler const&) noexcept = delete;
  ShaderCompiler(ShaderCompiler&&) noexcept = delete;
  auto operator=(ShaderCompiler const&) noexcept -> ShaderCompiler& = delete;
  auto operator=(ShaderCompiler&&) noexcept -> ShaderCompiler& = delete;
  ~ShaderCompiler() noexcept = default;

  [[nodiscard]] auto Compile() && noexcept -> GLuint;

 private:
  std::string vertex_shader_{};
  std::string fragment_shader_{};

  [[nodiscard]] auto LoadShaderFromFile(
      std::filesystem::path const& vertex_file_path,
      std::filesystem::path const& fragment_file_path) noexcept -> bool;
};

#endif  // SRC_SHADER_COMPILER_SHADER_COMPILER_HPP