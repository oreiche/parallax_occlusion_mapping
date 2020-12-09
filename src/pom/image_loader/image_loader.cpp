#include "FreeImagePlus.h"
#include "src/pom/image_loader/image_loader.hpp"

ImageLoader::ImageLoader() noexcept {
  FreeImage_Initialise();
  image_ = std::make_unique<fipImage>();
}

ImageLoader::~ImageLoader() noexcept { FreeImage_DeInitialise(); }

auto ImageLoader::load(std::filesystem::path const& img_path) noexcept -> bool {
  return image_->load(img_path.string().c_str()) == TRUE;
}

auto ImageLoader::data() const noexcept -> gsl::span<uint8_t const> {
  return gsl::span{image_->accessPixels(),
                   gsl::narrow<std::size_t>(pitch() * height())};
}

auto ImageLoader::width() const noexcept -> int {
  return gsl::narrow<int>(image_->getWidth());
}

auto ImageLoader::height() const noexcept -> int {
  return gsl::narrow<int>(image_->getHeight());
}

auto ImageLoader::pitch() const noexcept -> int {
  return gsl::narrow<int>(image_->getScanWidth());
}