cmake_minimum_required(VERSION 3.16)

project(kurs VERSION 0.1 LANGUAGES CXX)

# Включение автоматических систем Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Настройки стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск Qt с приоритетом Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Test)


# Список исходных файлов
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    notemanager.cpp
    notemanager.h
    noteformatter.cpp
    noteformatter.h
    noteexporter.cpp
    noteexporter.h
    emojimenubuilder.cpp
    emojimenubuilder.h
    recentlydeletedmanager.cpp
    recentlydeletedmanager.h
    recentlydeletedwindow.cpp
    recentlydeletedwindow.h
    noteitemdelegate.cpp
    noteitemdelegate.h
    note.h
)

# Список файлов ресурсов
set(QRC_FILES resources.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_resources(RESOURCE_FILES ${QRC_FILES})
else()
    qt5_add_resources(RESOURCE_FILES ${QRC_FILES})
endif()

# Создание основного GUI-приложения
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(kurs
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RESOURCE_FILES}
    )
else()
    if(ANDROID)
        add_library(kurs SHARED ${PROJECT_SOURCES} ${RESOURCE_FILES})
    else()
        add_executable(kurs ${PROJECT_SOURCES} ${RESOURCE_FILES})
    endif()
endif()

target_link_libraries(kurs PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

# Настройки для macOS и Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.kurs)
endif()

set_target_properties(kurs PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Установка
include(GNUInstallDirs)
install(TARGETS kurs
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Финализация (только для Qt6)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(kurs)
endif()

message(STATUS "Building with Qt ${QT_VERSION_MAJOR}")

# ====================
# === ЮНИТ-ТЕСТЫ =====
# ====================
enable_testing()

add_executable(test_manager
    test_notemanager.cpp

    notemanager.cpp
    noteexporter.cpp
    recentlydeletedmanager.cpp
    mainwindow.cpp
    noteformatter.cpp
    emojimenubuilder.cpp
    recentlydeletedwindow.cpp
    noteitemdelegate.cpp

    mainwindow.ui
)


target_link_libraries(test_manager
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)


add_test(NAME NoteManagerTests COMMAND tests)
