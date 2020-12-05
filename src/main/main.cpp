#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "src/renderer/renderer.hpp"
#include "src/shader_compiler/shader_compiler.hpp"

auto main() -> int {
  Renderer renderer{"Tutorial"};

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

  if (program_id) {
    auto render_status = renderer.Run([&] {
      // Use our shader
      glUseProgram(*program_id);

      // 1st attribute buffer : vertices
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glVertexAttribPointer(0,  // attribute 0. No particular reason for 0,
                                // but must match the layout in the shader.
                            3,  // size
                            GL_FLOAT,  // type
                            GL_FALSE,  // normalized?
                            0,         // stride
                            nullptr    // array buffer offset
      );

      // Draw the triangle !
      glDrawArrays(
          GL_TRIANGLES, 0,
          3);  // Starting from vertex 0; 3 vertices total -> 1 triangle
      glDisableVertexAttribArray(0);

      return true;  // Run forever
    });

    return (render_status ? EXIT_SUCCESS : EXIT_FAILURE);
  }

  return EXIT_FAILURE;
}
