# CMakeLists.txt (subdiretório src)

add_executable(headers_check ${CMAKE_SOURCE_DIR}/tests/test_headers.cpp)

target_include_directories(headers_check PRIVATE ${CMAKE_SOURCE_DIR}/include)
