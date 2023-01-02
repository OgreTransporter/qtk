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

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(/wd4131 /wd4127)
endif()

################################################################################
# Project
################################################################################
project(
    #[[NAME]]   Qtk
    VERSION     1.0
    DESCRIPTION "An example project using QT and OpenGL"
    LANGUAGES   CXX C
)

################################################################################
# Options
################################################################################
option(QTK_DEBUG "Enable debugger" ON)
option(QTK_UPDATE_SUBMODULES "Update external project (assimp) submodule" OFF)
option(QTK_SHARED "Build shared libraries" ON)
option(QTK_BUILD_GUI "Build the Qtk desktop application" ON)

# Options for bringing your own assimp installation; Otherwise not needed
# + If assimp is available system-wide we can just set QTK_UPDATE_SUBMODULES OFF
option(ASSIMP_NEW_INTERFACE "Use the assimp::assimp interface (WIN / OSX)" OFF)
message(STATUS
    "[Qtk] ASSIMP_NEW_INTERFACE=${ASSIMP_NEW_INTERFACE}"
)

set(QT_DIR "$ENV{HOME}/Code/Clones/Qt/" CACHE PATH "Path to Qt6")
# Qt Designer will look in different locations if WIN / Unix.
# These paths are for using Qt Designer integrated within Qt Creator.
# Standalone Qt Designer may use different paths.
if (WIN32)
  set(QT_PLUGIN_INSTALL_DIR
      "${QT_DIR}/Tools/QtCreator/bin/plugins/designer"
      )
  # This path may be different on windows. I have not tested this.
  set(QT_PLUGIN_LIBRARY_DIR
      "${QT_DIR}/Tools/QtCreator/lib/Qt/lib"
      )
else()
  set(QT_PLUGIN_INSTALL_DIR
      "${QT_DIR}/Tools/QtCreator/lib/Qt/plugins/designer"
      )
  set(QT_PLUGIN_LIBRARY_DIR
      "${QT_DIR}/Tools/QtCreator/lib/Qt/lib"
      )
endif()

# This should be set to your Qt6 installation directory.
set(QT_INSTALL_DIR "${QT_DIR}/6.3.1/gcc_64/" CACHE PATH "Path to Qt6 install")
# For CLion builds, point CMAKE_PREFIX_PATH to Qt6 install directory
# + QtCreator will handle this for you if that is used instead
list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_DIR}")

set(QTK_PLUGIN_LIBRARY_DIR "${QT_PLUGIN_LIBRARY_DIR}")
set(QTK_PLUGIN_INSTALL_DIR "${QT_PLUGIN_INSTALL_DIR}")
set(QTK_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
#set(QTK_INSTALL_PREFIX "${QT_INSTALL_DIR}")
#set(QTK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(QTK_RESOURCES "${CMAKE_SOURCE_DIR}/resources")

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
if(NOT Qt6_FOUND)
  message(SEND_ERROR
      "[Qtk] Error: Unable to find Qt6 at CMAKE_PREFIX_PATH: "
      "${CMAKE_PREFIX_PATH}"
  )
  message(FATAL_ERROR
      "[Qtk] Error: Specify path to Qt6 with `cmake "
      "-DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64 -S /path/to/qtk -B "
      "/path/to/qtk/build && cmake --build /path/to/qtk/build -j $(nprocs)`"
  )
endif()

# Find Assimp
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

if(WIN32)
  get_target_property(_qt6_qmake_location Qt6::qmake IMPORTED_LOCATION)
  execute_process(COMMAND "${_qt6_qmake_location}" -query QT_INSTALL_PREFIX
      RESULT_VARIABLE return_code
      OUTPUT_VARIABLE qt6_install_prefix
      OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  file(TO_NATIVE_PATH "${qt6_install_prefix}/bin" qt6_install_prefix)

  if(TARGET Qt6::windeployqt)
    add_custom_command(TARGET qtk-main
        POST_BUILD
        COMMAND set PATH=%PATH%$<SEMICOLON>${qt6_install_prefix}
        COMMAND Qt6::windeployqt --dir "${CMAKE_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:qtk-main>/$<TARGET_FILE_NAME:qtk-main>"
    )
    install(DIRECTORY "${CMAKE_BINARY_DIR}/windeployqt/" DESTINATION bin)
  endif()

  if(MSVC AND TARGET Qt6::qmake)
    set(VSUSER_FILE ${CMAKE_CURRENT_BINARY_DIR}/qtk-main.vcxproj.user)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/extern/assimp/assimp/bin" assimp_bin)
    file(WRITE ${VSUSER_FILE} "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
    file(APPEND ${VSUSER_FILE} "<Project xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n")
    file(APPEND ${VSUSER_FILE} "  <PropertyGroup>\n")
    file(APPEND ${VSUSER_FILE} "    <LocalDebuggerEnvironment>Path=$(SolutionDir)\\lib\\$(Configuration);${qt6_install_prefix};${assimp_bin};$(Path)\n")
    file(APPEND ${VSUSER_FILE} "$(LocalDebuggerEnvironment)</LocalDebuggerEnvironment>\n")
    file(APPEND ${VSUSER_FILE} "    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n")
    file(APPEND ${VSUSER_FILE} "  </PropertyGroup>\n")
    file(APPEND ${VSUSER_FILE} "</Project>\n")
  endif()

endif()
