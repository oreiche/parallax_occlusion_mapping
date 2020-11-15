#include <iostream>

#include "gsl/gsl-lite.hpp"

auto main() -> int {
  gsl::owner<int*> ptr{new int};
  std::cout << "Hello World" << std::endl;
  delete ptr;

  return 0;
}