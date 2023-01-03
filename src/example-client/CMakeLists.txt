################################################################################
## Example client project using qtk                                           ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################
cmake_minimum_required(VERSION 3.23)

################################################################################
# Constants
################################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
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
    #[[NAME]]   QtkClient
    VERSION     1.0
    DESCRIPTION "An example project using Qtk"
    LANGUAGES   CXX C
)

# If you did not install Qtk on a system path, point cmake to installation.
#list(APPEND CMAKE_PREFIX_PATH /home/shaun/Code/qtk/install)

find_package(Qtk 1.0 REQUIRED)

# Print all QTK variables
get_cmake_property(VAR_NAMES VARIABLES)
list(FILTER VAR_NAMES INCLUDE REGEX "^QTK_.*$")
list(SORT VAR_NAMES)
foreach(VAR_NAME ${VAR_NAMES})
  message(STATUS "[Qtk] ${VAR_NAME}=${${VAR_NAME}}")
endforeach()

set(QT_INSTALL_DIR "/home/kapper/Code/Clones/Qt/6.3.1/gcc_64/" CACHE PATH "Path to Qt6 install")
list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_DIR}")
find_package(Qt6 COMPONENTS Core Widgets OpenGLWidgets REQUIRED)

set(EXAMPLE_SOURCES
    main.cpp
    examplescene.cpp examplescene.h
    examplewidget.cpp examplewidget.h
)
qt_add_resources(EXAMPLE_SOURCES ../../resources/resources.qrc)

add_executable(example ${EXAMPLE_SOURCES})
target_link_libraries(example PUBLIC Qt6::Widgets Qt6::OpenGLWidgets Qt6::Core)
target_link_libraries(example PUBLIC Qtk::qtk-library)
