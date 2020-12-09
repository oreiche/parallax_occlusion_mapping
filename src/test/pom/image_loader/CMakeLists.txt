add_executable(image_loader.test image_loader.test.cpp)
target_link_libraries(image_loader.test PRIVATE test_main image_loader)

add_memchk_test(image_loader image_loader.test)