#ifndef SRC_POM_COMMON_COMMON_HPP
#define SRC_POM_COMMON_COMMON_HPP

#include <cstdlib>
#include <filesystem>
#include <iostream>

[[nodiscard]] static inline auto SetSailCodecsPath() noexcept -> bool {
  std::filesystem::path const codecs_path{
#ifdef SAIL_MY_CODECS_PATH
      SAIL_MY_CODECS_PATH
#else
      "codecs"
#endif
  };

  try {
    if (std::filesystem::is_directory(codecs_path)) {
      return setenv("SAIL_MY_CODECS_PATH", codecs_path.c_str(), 0) == 0;
    }
  } catch (...) {
    std::cerr << "Failed to set environment varialbe" << std::endl;
  }
  return false;
}

#endif  // SRC_POM_COMMON_COMMON_HPP
