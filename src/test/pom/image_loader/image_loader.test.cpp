#include "catch2/catch.hpp"
#include "gsl/gsl-lite.hpp"
#include "src/pom/image_loader/image_loader.hpp"

TEST_CASE("Read PNG image", "[image_loader]") {
  static constexpr uint8_t kBlack = 0;
  static constexpr uint8_t kWhite = 255;

  ImageLoader img{};
  REQUIRE(img.load("data/test_image.png"));
  CHECK(img.width() == 2);
  CHECK(img.height() == 2);
  CHECK(img.pitch() == 8);

  auto pixels = img.data();
  REQUIRE_FALSE(pixels.data() == nullptr);
  CHECK(pixels[0] == kWhite);
  CHECK(pixels[4] == kBlack);
  CHECK(pixels[8] == kBlack);
  CHECK(pixels[12] == kWhite);
}