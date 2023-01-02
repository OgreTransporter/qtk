################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################

# TODO: Provide option for linking MainWindow with plugin statically
# TODO: QtkWidget plugin should instantiate a ptr to DebugConsole and store it as a member.
#       Then we can connect a signal from MainWindow for creating a console to QtkWidget
#       When triggered QtkWidget slot will send signal to MainWindow
#       MainWindow catches signal and runs slot to attach DebugConsole to MainWindow as QDockWidget
# TODO: Create a second repository for working on QtkApplication (MainWindow)
################################################################################
# Qtk Library
################################################################################
set(PUBLIC_HEADERS
    camera3d.h
    input.h
    shape.h
    meshrenderer.h
    model.h
    modelmesh.h
    object.h
    qtkapi.h
    scene.h
    skybox.h
    texture.h
    transform3D.h
)

set(SOURCE_FILES
    camera3d.cpp
    input.cpp
    shape.cpp
    meshrenderer.cpp
    model.cpp
    modelmesh.cpp
    object.cpp
    scene.cpp
    skybox.cpp
    texture.cpp
    transform3D.cpp
)

qt_add_library(qtk-library SHARED)
target_sources(qtk-library PRIVATE ${SOURCE_FILES})
target_sources(qtk-library PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/src
    FILES ${PUBLIC_HEADERS}
)

if(QTK_DEBUG)
  target_compile_definitions(qtk-library PUBLIC QTK_DEBUG)
endif()
if(QTK_SHARED)
  target_compile_definitions(qtk-library PUBLIC QTK_SHARED)
endif()

set_target_properties(qtk-library PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    VERSION ${PROJECT_VERSION}
)

target_link_libraries(qtk-library PUBLIC
    Qt6::Core Qt6::OpenGLWidgets Qt6::Widgets
)

if(QTK_UPDATE_SUBMODULES OR NOT ASSIMP_NEW_INTERFACE)
  target_link_libraries(qtk-library PUBLIC assimp)
elseif(ASSIMP_NEW_INTERFACE)
  target_link_libraries(qtk-library PUBLIC assimp::assimp)
endif()

if(WIN32)
  target_link_libraries(qtk-library PUBLIC OpenGL::GL)
endif()

# System install for qtk-library
# TODO: Use RUNTIME_DEPENDENCY_SET to install Qt libraries for local builds?
install(TARGETS qtk-library
    # Associate qtk-library target with qtk-export
    EXPORT qtk-export
    FILE_SET HEADERS DESTINATION "${QTK_INSTALL_PREFIX}/include"
    BUNDLE DESTINATION ${QTK_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${QTK_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${QTK_INSTALL_PREFIX}/lib/static
    RUNTIME DESTINATION ${QTK_INSTALL_PREFIX}/bin
)

## Install qtk-library to Qt Designer to support widget plugins.
install(TARGETS qtk-library
    BUNDLE DESTINATION ${QTK_PLUGIN_LIBRARY_DIR}
    LIBRARY DESTINATION ${QTK_PLUGIN_LIBRARY_DIR}
    ARCHIVE DESTINATION ${QTK_PLUGIN_LIBRARY_DIR}
    RUNTIME DESTINATION ${QTK_PLUGIN_LIBRARY_DIR}
)
