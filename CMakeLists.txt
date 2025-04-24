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
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)  # Добавлен Core для работы с ресурсами
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

# Список исходных файлов
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Список файлов ресурсов (.qrc)
set(QRC_FILES
    resources.qrc  # Добавьте другие .qrc файлы при необходимости
)

# Обработка ресурсов в зависимости от версии Qt
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_resources(RESOURCE_FILES ${QRC_FILES})
else()
    qt5_add_resources(RESOURCE_FILES ${QRC_FILES})
endif()

# Создание исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(kurs
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RESOURCE_FILES}  # Добавлены ресурсы
    )
else()
    if(ANDROID)
        add_library(kurs SHARED
            ${PROJECT_SOURCES}
            ${RESOURCE_FILES}  # Добавлены ресурсы
        )
    else()
        add_executable(kurs
            ${PROJECT_SOURCES}
            ${RESOURCE_FILES}  # Добавлены ресурсы
        )
    endif()
endif()

# Линковка с Qt компонентами
target_link_libraries(kurs PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core  # Добавлен Core для работы с ресурсами
)

# Настройки для macOS
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

# Финализация для Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(kurs)
endif()

# Вывод информации о используемой версии Qt (для отладки)
message(STATUS "Building with Qt ${QT_VERSION_MAJOR}")
