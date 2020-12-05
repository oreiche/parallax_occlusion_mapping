#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "src/pom/renderer/renderer.hpp"
#include "src/pom/shader_compiler/shader_compiler.hpp"

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
uniform mat4 MVP; \
void main() { \
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0); \
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
    static constexpr auto kFoV = glm::radians(45.0F);
    static constexpr auto kAspectRatio = 16.0F / 9;
    static constexpr auto kClipNear = 0.1F;
    static constexpr auto kClipFar = 100.0F;

    // model matrix
    auto model_matrix =
        glm::translate(glm::mat4{1.0F},               // identity matrix
                       glm::vec3(0.0F, 0.0F, 0.0F));  // translation vector

    // view matrix
    auto view_matrix = glm::lookAt(
        glm::vec3(1.0F, 1.0F, 5.0F),   // camera position NOLINT
        glm::vec3(0.0F, 0.0F, 0.0F),   // camera target
        glm::vec3(0.0F, 1.0F, 0.0F));  // vector defining camera 'up'

    // projection matrix
    auto projection_matrix =
        glm::perspective(kFoV, kAspectRatio, kClipNear, kClipFar);

    // accumulated mode view projection matrix
    auto mvp_matrix = projection_matrix * view_matrix * model_matrix;

    // handle to 'MVP' uniform in vertex shader
    auto mvp_id = glGetUniformLocation(*program_id, "MVP");

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

      // upload model view projection matrix
      glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp_matrix[0][0]);

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
