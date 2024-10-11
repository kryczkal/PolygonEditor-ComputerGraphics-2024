cmake_minimum_required(VERSION 3.16)

project(PolygonEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Copy assets to build directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Add include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files in src folder
set(PROJECT_SOURCES
    src/main.cpp
        src/MainWindow.cpp
        src/PolygonItem.cpp
        include/MainWindow.h
        include/PolygonItem.h
        include/VertexItem.h
        src/EdgeItem.cpp
        src/VertexItem.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PolygonEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(PolygonEditor SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(PolygonEditor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(PolygonEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PolygonEditor)
endif()
set_target_properties(PolygonEditor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PolygonEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PolygonEditor)
endif()
