#include <filesystem>

#include "catch2/catch.hpp"
#include "src/shader_compiler/shader_compiler.hpp"

TEST_CASE("Compile shaders from files", "[shader_compiler]") {
  SECTION("Both valid shaders") {
    std::filesystem::path vertex{"data/valid_vertex.glsl"};
    std::filesystem::path fragment{"data/valid_fragment.glsl"};

#ifdef WITH_OPENGL
    CHECK(ShaderCompiler{vertex, fragment}.Compile());
#else
    ShaderCompiler compiler{vertex, fragment};
    CHECK(compiler.IsInitialized());
#endif
  }

  SECTION("Faulty vertex shader") {
    std::filesystem::path vertex{"data/faulty_vertex.glsl"};
    std::filesystem::path fragment{"data/valid_fragment.glsl"};

#ifdef WITH_OPENGL
    CHECK(!ShaderCompiler{vertex, fragment}.Compile());
#else
    ShaderCompiler compiler{vertex, fragment};
    CHECK(compiler.IsInitialized());
#endif
  }

  SECTION("Faulty vertex shader") {
    std::filesystem::path vertex{"data/valid_vertex.glsl"};
    std::filesystem::path fragment{"data/faulty_fragment.glsl"};

#ifdef WITH_OPENGL
    CHECK(!ShaderCompiler{vertex, fragment}.Compile());
#else
    ShaderCompiler compiler{vertex, fragment};
    CHECK(compiler.IsInitialized());
#endif
  }

  SECTION("Both faulty shaders") {
    std::filesystem::path vertex{"data/faulty_vertex.glsl"};
    std::filesystem::path fragment{"data/faulty_fragment.glsl"};

#ifdef WITH_OPENGL
    CHECK(!ShaderCompiler{vertex, fragment}.Compile());
#else
    ShaderCompiler compiler{vertex, fragment};
    CHECK(compiler.IsInitialized());
#endif
  }
}

#ifdef WITH_OPENGL

TEST_CASE("Compile shaders from string", "[shader_compiler]") {
  std::string const kValidVertexShader =
      "#version 330 core\n \
void main() { \
  gl_Position.xyzw = vec4(0, 0, 0, 0); \
}";

  std::string const kValidFragmentShader =
      "#version 330 core\n \
out vec3 color; \
void main() { \
  color = vec3(0, 0, 0); \
}";

  std::string const kFaultyVertexShader =
      "#version 330 core\n \
void main() { \
  gl_Position.xyzw = vec3(0, 0, 0); \
}";

  std::string const kFaultyFragmentShader =
      "#version 330 core\n \
out vec3 color; \
void main() { \
  color = vec3(0, 0, 0, 0); \
}";

  SECTION("Both valid shaders") {
    CHECK(ShaderCompiler{kValidVertexShader, kValidFragmentShader}.Compile());
  }

  SECTION("Faulty vertex shader") {
    CHECK(!ShaderCompiler{kFaultyVertexShader, kValidFragmentShader}.Compile());
  }

  SECTION("Faulty vertex shader") {
    CHECK(!ShaderCompiler{kValidVertexShader, kFaultyFragmentShader}.Compile());
  }

  SECTION("Both faulty shaders") {
    CHECK(
        !ShaderCompiler{kFaultyVertexShader, kFaultyFragmentShader}.Compile());
  }
}

#endif  // WITH_OPENGL
