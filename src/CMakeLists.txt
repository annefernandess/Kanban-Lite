# CMakeLists.txt (subdiretório src)

# Coletora arquivos fonte das implementações
set(KANBAN_SOURCES
    User.cpp
    ActivityLog.cpp
    Card.cpp
    Column.cpp
    Board.cpp
)

# Executável de teste de headers (Etapa 1)
add_executable(headers_check ${CMAKE_SOURCE_DIR}/tests/test_headers.cpp)
target_include_directories(headers_check PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Executável CLI principal (Etapa 2)
add_executable(kanban_cli main.cpp ${KANBAN_SOURCES})
target_include_directories(kanban_cli PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Configurações de compilação específicas para kanban_cli
target_compile_features(kanban_cli PRIVATE cxx_std_17)

# Para sistemas que requerem pthread explicitamente
find_package(Threads REQUIRED)
target_link_libraries(kanban_cli PRIVATE Threads::Threads)
