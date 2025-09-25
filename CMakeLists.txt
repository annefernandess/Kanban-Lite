cmake_minimum_required(VERSION 3.16)
project(kanban_lite LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(KANBAN_LITE_ENABLE_WARNINGS "Ativa flags de warning estritos" ON)

if(KANBAN_LITE_ENABLE_WARNINGS)
	if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)
	endif()
endif()

add_subdirectory(src)

include(CTest)
enable_testing()

add_test(NAME headers_check COMMAND headers_check)
