cmake_minimum_required(VERSION 3.16)

project(PolygonEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/71815bbf7de6e10c11821d654a2fae2cf42de0f7.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

# Copy assets to build directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Add include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files in src folder
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp include/*.h)
set(PROJECT_SOURCES ${PROJECT_SOURCES})

# Create a library for shared code
add_library(PolygonEditorLib
        ${PROJECT_SOURCES}
)

# Link Qt Widgets to the library
target_link_libraries(PolygonEditorLib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Add the main executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PolygonEditor
            MANUAL_FINALIZATION
            src/main.cpp
    )
else()
    if(ANDROID)
        add_library(PolygonEditor SHARED
                src/main.cpp
        )
    else()
        add_executable(PolygonEditor
                src/main.cpp
        )
    endif()
endif()

# Include Qt directories for the main executable
target_include_directories(PolygonEditor PRIVATE ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})

# Link the library to the main executable
target_link_libraries(PolygonEditor PRIVATE PolygonEditorLib Qt${QT_VERSION_MAJOR}::Widgets)

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

# Google Test setup
enable_testing()
file(GLOB TEST_SOURCES tests/*.cc)
add_executable(run_google_tests ${TEST_SOURCES})

# Include Qt directories for the test executable
target_include_directories(run_google_tests PRIVATE ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})

# Link the test executable with Google Test and the shared library
target_link_libraries(run_google_tests PRIVATE GTest::gtest_main PolygonEditorLib Qt${QT_VERSION_MAJOR}::Widgets)

include(GoogleTest)
gtest_discover_tests(run_google_tests)