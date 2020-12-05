#include "catch2/catch.hpp"
#include "src/pom/renderer/renderer.hpp"

#ifdef WITH_OPENGL

TEST_CASE("Run renderer with empty drawing callback", "[renderer]") {
  Renderer renderer{"test"};

  bool draw_called{false};

  Renderer::DrawCallback callback{[&] {
    draw_called = true;
    return false;  // abort
  }};

  CHECK(renderer.Run(callback));
  CHECK(draw_called);
}

#endif  // WITH_OPENGL