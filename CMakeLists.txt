################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################
cmake_minimum_required(VERSION 3.23)

################################################################################
# Includes
################################################################################
include("${CMAKE_SOURCE_DIR}/cmake/include/git_submodule.cmake")
include(CMakePackageConfigHelpers)

################################################################################
# Constants
################################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MACOSX_BUNDLE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
  set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)
  add_compile_options(/wd4131 /wd4127)
endif()

################################################################################
# Project
################################################################################
project(
    #[[NAME]]   Qtk
    VERSION     0.2
    DESCRIPTION "Qt OpenGL library and desktop application."
    LANGUAGES   CXX C
)

################################################################################
# Options
################################################################################
option(QTK_DEBUG "Enable debugger" ON)
option(QTK_BUILD_GUI "Build the Qtk desktop application" ON)
option(QTK_BUILD_EXAMPLE "Build the Qtk example desktop application" ON)
option(QTK_UPDATE_SUBMODULES "Update external project (assimp) submodule" OFF)
# Install Qtk for use within Qt Creator projects only, instead of system-wide.
option(QTK_PREFIX_QTCREATOR "Install Qtk to Qt Creator." OFF)
# Option for bringing your own assimp installation; Otherwise not needed
# + If assimp is available system-wide we can just set QTK_UPDATE_SUBMODULES OFF
option(
    QTK_ASSIMP_NEW_INTERFACE
    "Use the assimp::assimp interface (WIN / OSX)"
    OFF
)

if(NOT QTK_DEBUG)
  set(CMAKE_BUILD_TYPE Release)
else()
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(QT_DIR "$ENV{HOME}/Qt/" CACHE PATH "Path to Qt6")

# Qt Designer will look in different locations if WIN / Unix.
# These paths are for using Qt Designer integrated within Qt Creator.
# Standalone Qt Designer may use different paths.
if (WIN32)
  # These paths may be different on windows. I have not tested this.
  set(QT_PLUGIN_INSTALL_DIR "${QT_DIR}/Tools/QtCreator/bin/plugins/designer")
  set(QT_PLUGIN_LIBRARY_DIR "${QT_DIR}/Tools/QtCreator/lib/Qt/lib")
else()
  set(QT_PLUGIN_INSTALL_DIR "${QT_DIR}/Tools/QtCreator/lib/Qt/plugins/designer")
  set(QT_PLUGIN_LIBRARY_DIR "${QT_DIR}/Tools/QtCreator/lib/Qt/lib")
endif()

# This should be set to your Qt6 installation directory.
set(QT_INSTALL_DIR "${QT_DIR}/6.5.0/gcc_64" CACHE PATH "Path to Qt6 install")
# Point CMAKE_PREFIX_PATH to Qt6 install directory
# If Qtk is built within Qt Creator this is not required.
list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_DIR}")

set(QTK_PLUGIN_LIBRARY_DIR "${QT_PLUGIN_LIBRARY_DIR}")
set(QTK_PLUGIN_INSTALL_DIR "${QT_PLUGIN_INSTALL_DIR}")
set(CMAKE_INSTALL_PREFIX  "${CMAKE_SOURCE_DIR}/install")
if (QTK_PREFIX_QTCREATOR)
  # TODO: This might be a bit strange and needs more testing.
  set(CMAKE_INSTALL_PREFIX  "${QT_INSTALL_DIR}")
endif()
message(STATUS "[Qtk] CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
set(QTK_RESOURCES "${CMAKE_SOURCE_DIR}/resources")
set(QTK_OSX_ICONS ${CMAKE_SOURCE_DIR}/resources/icons/osx/kilroy.icns)

# Print all QTK options and their values.
get_cmake_property(VAR_NAMES VARIABLES)
list(FILTER VAR_NAMES INCLUDE REGEX "^QTK_.*$")
list(SORT VAR_NAMES)
foreach(VAR_NAME ${VAR_NAMES})
  message(STATUS "[Qtk] ${VAR_NAME}=${${VAR_NAME}}")
endforeach()

################################################################################
# External Dependencies
################################################################################
# Find Qt
find_package(Qt6 COMPONENTS Core UiPlugin OpenGLWidgets)
qt_standard_project_setup()
if(NOT Qt6_FOUND)
  message(
      SEND_ERROR "[Qtk] Error: Unable to find Qt6 at CMAKE_PREFIX_PATH: "
      "${CMAKE_PREFIX_PATH}"
  )
  message(
      FATAL_ERROR "[Qtk] Error: Specify path to Qt6 with `cmake "
      "-DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64 -S /path/to/qtk -B "
      "/path/to/qtk/build && cmake --build /path/to/qtk/build -j $(nprocs)`"
  )
endif()

# Find Assimp
set(ASSIMP_BUILD_TESTS OFF CACHE STRING "Do not build assimp tests." FORCE)
set(ASSIMP_INSTALL ON CACHE STRING "Use assimp as a submodule." FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE STRING "No warnings as errors." FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE STRING "Do not postfix assimp with -dev" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE STRING "Build static assimp libs" FORCE)
set(ASSIMP_OPT_BUILD_PACKAGES ON CACHE STRING "Build static assimp libs" FORCE)
if(QTK_UPDATE_SUBMODULES)
  message(STATUS "[Qtk] Updating submodules...")
  submodule_update("${CMAKE_CURRENT_SOURCE_DIR}/extern/assimp/assimp/")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/assimp/assimp/")
else()
  find_package(assimp REQUIRED)
endif()

if(WIN32)
  find_package(OpenGL REQUIRED)
endif()

################################################################################
# Qtk
################################################################################
add_subdirectory(src)

if(QTK_BUILD_EXAMPLE)
  # Create a namespaced alias for linking with qtk_library in the example.
  add_library(${PROJECT_NAME}::qtk_library ALIAS qtk_library)
  add_subdirectory(example-app)
endif()
