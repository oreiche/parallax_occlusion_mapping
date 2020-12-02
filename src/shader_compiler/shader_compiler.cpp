#include "shader_compiler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "gsl/gsl-lite.hpp"

auto ShaderCompiler::Compile() && noexcept -> std::optional<GLuint> {
  if (!IsInitialized()) {
    return std::nullopt;
  }

  // Create the shaders
  GLuint vertex_id{glCreateShader(GL_VERTEX_SHADER)};
  GLuint fragment_id{glCreateShader(GL_FRAGMENT_SHADER)};

  GLint result{GL_FALSE};
  int msg_length{};

  // Compile Vertex Shader
  std::cout << "Compiling vertex shader" << std::endl;
  auto const* vertex_shader_ptr = vertex_shader_.c_str();
  glShaderSource(vertex_id, 1, &vertex_shader_ptr, nullptr);
  glCompileShader(vertex_id);

  // Check Vertex Shader
  glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &msg_length);
  if (msg_length > 0) {
    std::string error_msg{"\0", gsl::narrow<std::size_t>(msg_length + 1)};
    glGetShaderInfoLog(vertex_id, msg_length, nullptr, error_msg.data());
    std::cerr << error_msg.c_str() << std::endl;
  }
  if (result == GL_FALSE) {
    return std::nullopt;
  }

  // Compile Fragment Shader
  std::cout << "Compiling fragment shader" << std::endl;
  auto const* fragment_shader_ptr = fragment_shader_.c_str();
  glShaderSource(fragment_id, 1, &fragment_shader_ptr, nullptr);
  glCompileShader(fragment_id);

  // Check Fragment Shader
  glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &msg_length);
  if (msg_length > 0) {
    std::string error_msg{"\0", gsl::narrow<std::size_t>(msg_length + 1)};
    glGetShaderInfoLog(fragment_id, msg_length, nullptr, error_msg.data());
    std::cerr << error_msg.c_str() << std::endl;
  }
  if (result == GL_FALSE) {
    return std::nullopt;
  }

  // Link the program
  std::cout << "Linking program" << std::endl;
  GLuint program_id{glCreateProgram()};
  glAttachShader(program_id, vertex_id);
  glAttachShader(program_id, fragment_id);
  glLinkProgram(program_id);

  // Check the program
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &msg_length);
  if (msg_length > 0) {
    std::string error_msg{"\0", gsl::narrow<std::size_t>(msg_length + 1)};
    glGetProgramInfoLog(program_id, msg_length, nullptr, error_msg.data());
    std::cerr << error_msg.c_str() << std::endl;
  }
  if (result == GL_FALSE) {
    return std::nullopt;
  }

  glDetachShader(program_id, vertex_id);
  glDetachShader(program_id, fragment_id);

  glDeleteShader(vertex_id);
  glDeleteShader(fragment_id);

  return program_id;
}

auto ShaderCompiler::LoadShaderFromFile(
    std::filesystem::path const& vertex_file_path,
    std::filesystem::path const& fragment_file_path) noexcept -> bool {
  try {
    // Read the Vertex Shader code from the file
    std::ifstream vertex_stream(vertex_file_path, std::ios::in);
    if (vertex_stream.is_open()) {
      std::stringstream sstr;
      sstr << vertex_stream.rdbuf();
      vertex_shader_ = sstr.str();
      vertex_stream.close();
    } else {
      std::cerr << "Cannot find " << vertex_file_path.string() << std::endl;
      return false;
    }

    // Read the Fragment Shader code from the file
    std::ifstream fragment_stream(fragment_file_path, std::ios::in);
    if (fragment_stream.is_open()) {
      std::stringstream sstr;
      sstr << fragment_stream.rdbuf();
      fragment_shader_ = sstr.str();
      fragment_stream.close();
    } else {
      std::cerr << "Cannot find " << fragment_file_path.string() << std::endl;
    }
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }

  return true;
}
