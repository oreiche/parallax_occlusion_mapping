#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "src/pom/image_loader/image_loader.hpp"
#include "src/pom/renderer/renderer.hpp"
#include "src/pom/shader_compiler/shader_compiler.hpp"

auto main() -> int {
  ImageLoader checkerboard{};
  ImageLoader depth_map{};

  if (!checkerboard.load("img/checkerboard.png")) {
    std::cerr << "Could not load image img/checkerboard.png" << std::endl;
    return EXIT_FAILURE;
  }

  if (!depth_map.load("img/depth_map.png")) {
    std::cerr << "Could not load image img/depth_map.png" << std::endl;
    return EXIT_FAILURE;
  }

  Renderer renderer{"Tutorial"};

  GLuint VertexArrayID{};
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // An array of 3 vectors which represents 3 vertices
  std::vector<GLfloat> g_vertex_buffer_data{
      -1.0F, -1.0F, 0.0F,  // t1 v1
      +1.0F, -1.0F, 0.0F,  // t1 v2
      -1.0F, +1.0F, 0.0F,  // t1 v3
      -1.0F, +1.0F, 0.0F,  // t2 v1
      +1.0F, +1.0F, 0.0F,  // t2 v2
      +1.0F, -1.0F, 0.0F,  // t2 v3
  };

  // This will identify our vertex buffer
  GLuint vertex_buffer_id{};
  // Generate 1 buffer, put the resulting identifier in vertex_buffer_id
  glGenBuffers(1, &vertex_buffer_id);
  // The following commands will talk about our 'vertex_buffer_id' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER,
               gsl::narrow<GLsizeiptr>(
                   g_vertex_buffer_data.size() *
                   sizeof(decltype(g_vertex_buffer_data)::value_type)),
               g_vertex_buffer_data.data(), GL_STATIC_DRAW);

  // Create one OpenGL texture
  GLuint textureID{};
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will modify
  // this texture
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkerboard.width(),
               checkerboard.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               checkerboard.data().data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  GLuint depthmapID{};
  glGenTextures(1, &depthmapID);
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, depthmapID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, depth_map.width(), depth_map.height(),
               0, GL_RGBA, GL_UNSIGNED_BYTE, depth_map.data().data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  std::vector<GLfloat> g_tex_coord{
      0.0F, 0.0F,  // map t1
      1.0F, 0.0F,  // map t1
      0.0F, 1.0F,  // map t1
      0.0F, 1.0F,  // map t2
      1.0F, 1.0F,  // map t2
      1.0F, 0.0F,  // map t2
  };

  GLuint tex_coord_id{};
  glGenBuffers(1, &tex_coord_id);
  glBindBuffer(GL_ARRAY_BUFFER, tex_coord_id);
  glBufferData(
      GL_ARRAY_BUFFER,
      gsl::narrow<GLsizeiptr>(g_tex_coord.size() *
                              sizeof(decltype(g_tex_coord)::value_type)),
      g_tex_coord.data(), GL_STATIC_DRAW);

  std::string vertex_shader_code =
      "#version 330 core\n \
layout(location = 0) in vec3 vertexPosition_modelspace; \
layout(location = 1) in vec2 vertexUV; \
uniform mat4 MVP; \
out vec2 tex_coord; \
void main() { \
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0); \
  tex_coord = vertexUV; \
}";

  std::string fragment_shader_code =
      "#version 330 core\n \
in vec2 tex_coord; \
out vec3 color; \
uniform sampler2D tex; \
uniform sampler2D depth_map; \
void main() { \
  color = texture(tex, tex_coord).rgb \
        + texture(depth_map, tex_coord).rgb; \
}";

  // Create and compile our GLSL program from the shaders
  auto program_id =
      ShaderCompiler{vertex_shader_code, fragment_shader_code}.Compile();

  if (program_id) {
    static constexpr auto kFoV = glm::radians(45.0F);
    static constexpr auto kAspectRatio = 4.0F / 3;
    static constexpr auto kClipNear = 0.1F;
    static constexpr auto kClipFar = 100.0F;

    // model matrix
    auto model_matrix =
        glm::translate(glm::mat4{1.0F},               // identity matrix
                       glm::vec3(0.0F, 0.0F, 0.0F));  // translation vector

    // projection matrix
    auto projection_matrix =
        glm::perspective(kFoV, kAspectRatio, kClipNear, kClipFar);

    // handle to 'MVP' uniform in vertex shader
    auto mvp_id = glGetUniformLocation(*program_id, "MVP");
    auto tex_id = glGetUniformLocation(*program_id, "tex");
    auto map_id = glGetUniformLocation(*program_id, "depth_map");

    auto render_status = renderer.Run([&] {
      // Use our shader
      glUseProgram(*program_id);

      // 1st attribute buffer : vertices
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
      glVertexAttribPointer(0,  // attribute 0. No particular reason for 0,
                                // but must match the layout in the shader.
                            3,  // size
                            GL_FLOAT,  // type
                            GL_FALSE,  // normalized?
                            0,         // stride
                            nullptr    // array buffer offset
      );

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, tex_coord_id);
      glVertexAttribPointer(1,  // attribute 1. No particular reason for 1,
                                // but must match the layout in the shader.
                            2,  // size
                            GL_FLOAT,  // type
                            GL_TRUE,   // normalized?
                            0,         // stride
                            nullptr    // array buffer offset
      );

      // accumulated mode view projection matrix
      auto mvp_matrix =
          projection_matrix * renderer.GetViewMatrix() * model_matrix;

      // upload model view projection matrix
      glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp_matrix[0][0]);
      glUniform1i(tex_id, 0);
      glUniform1i(map_id, 1);

      // Draw the triangle !
      glDrawArrays(
          GL_TRIANGLES, 0,
          2 * 3  // Starting from vertex 0; 6 vertices total -> 2 triangles
      );
      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);

      return true;  // Run forever
    });

    return (render_status ? EXIT_SUCCESS : EXIT_FAILURE);
  }

  return EXIT_FAILURE;
}
