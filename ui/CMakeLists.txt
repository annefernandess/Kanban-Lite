# GUI executável com Qt

set(GUI_SOURCES
    main_gui.cpp
    MainWindow.cpp
    BoardView.cpp
    ColumnView.cpp
    CardView.cpp
    CardEditDialog.cpp
)

set(GUI_HEADERS
    MainWindow.h
    BoardView.h
    ColumnView.h
    CardView.h
    CardEditDialog.h
)

# Cria executável GUI
add_executable(kanban_lite_gui ${GUI_SOURCES} ${GUI_HEADERS})

# Link com as classes do domínio
target_link_libraries(kanban_lite_gui PRIVATE kanban_lib)

# Link com Qt
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(kanban_lite_gui PRIVATE Qt6::Widgets)
else()
    target_link_libraries(kanban_lite_gui PRIVATE Qt5::Widgets)
endif()

# Include paths
target_include_directories(kanban_lite_gui PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/external
)

# Propriedades do executável
set_target_properties(kanban_lite_gui PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Instalação
install(TARGETS kanban_lite_gui
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# Testes GUI (opcional, apenas se BUILD_TESTING)
if(BUILD_TESTING)
    add_executable(gui_tests ${CMAKE_SOURCE_DIR}/tests/gui_tests.cpp
                              ${GUI_SOURCES} ${GUI_HEADERS})
    target_link_libraries(gui_tests PRIVATE kanban_lib)
    if(QT_VERSION_MAJOR EQUAL 6)
        target_link_libraries(gui_tests PRIVATE Qt6::Widgets)
    else()
        target_link_libraries(gui_tests PRIVATE Qt5::Widgets)
    endif()
    target_include_directories(gui_tests PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/external
    )
    add_test(NAME gui_tests COMMAND gui_tests)
endif()
