#ifndef SRC_POM_IMAGE_LOADER_IMAGE_LOADER_HPP
#define SRC_POM_IMAGE_LOADER_IMAGE_LOADER_HPP

#include <filesystem>
#include <memory>

#include "gsl/gsl-lite.hpp"

class fipImage;

class ImageLoader {
 public:
  ImageLoader() noexcept;
  ImageLoader(ImageLoader const&) noexcept = delete;
  ImageLoader(ImageLoader&&) noexcept = delete;
  auto operator=(ImageLoader const&) noexcept -> ImageLoader& = delete;
  auto operator=(ImageLoader&&) noexcept -> ImageLoader& = delete;
  ~ImageLoader() noexcept;

  [[nodiscard]] auto load(std::filesystem::path const& img_path) noexcept
      -> bool;
  [[nodiscard]] auto data() const noexcept -> gsl::span<uint8_t const>;
  [[nodiscard]] auto width() const noexcept -> int;
  [[nodiscard]] auto height() const noexcept -> int;
  [[nodiscard]] auto pitch() const noexcept -> int;

 private:
  std::unique_ptr<fipImage> image_;
};

#endif  // SRC_POM_IMAGE_LOADER_IMAGE_LOADER_HPP