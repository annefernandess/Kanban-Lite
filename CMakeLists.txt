cmake_minimum_required(VERSION 3.16)
project(kanban_lite LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(KANBAN_LITE_ENABLE_WARNINGS "Ativa flags de warning estritos" ON)
option(KANBAN_LITE_BUILD_GUI "Compila versão GUI com Qt" ON)

if(KANBAN_LITE_ENABLE_WARNINGS)
	if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)
	endif()
endif()

# Qt integration para GUI
if(KANBAN_LITE_BUILD_GUI)
	# Tenta Qt6 primeiro, fallback para Qt5
	find_package(Qt6 COMPONENTS Widgets QUIET)
	if(Qt6_FOUND)
		message(STATUS "Usando Qt6 para GUI")
		set(QT_VERSION_MAJOR 6)
		set(QT_FOUND TRUE)
	else()
		find_package(Qt5 COMPONENTS Widgets QUIET)
		if(Qt5_FOUND)
			message(STATUS "Usando Qt5 para GUI")
			set(QT_VERSION_MAJOR 5)
			set(QT_FOUND TRUE)
		else()
			message(WARNING "Qt5/Qt6 não encontrado. Compilando apenas CLI.")
			set(QT_FOUND FALSE)
		endif()
	endif()
	
	if(QT_FOUND)
		# Ativa processamento automático do MOC, UIC e RCC
		set(CMAKE_AUTOMOC ON)
		set(CMAKE_AUTORCC ON)
		set(CMAKE_AUTOUIC ON)
		
		add_subdirectory(ui)
		message(STATUS "Interface gráfica será compilada")
	else()
		message(STATUS "Interface gráfica não será compilada (Qt não encontrado)")
	endif()
endif()

add_subdirectory(src)

include(CTest)
enable_testing()

add_test(NAME headers_check COMMAND headers_check)
add_test(NAME cli_tests 
         COMMAND ${CMAKE_SOURCE_DIR}/tests/cli_tests.sh
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src)
add_test(NAME persistence_tests COMMAND persistence_tests)
