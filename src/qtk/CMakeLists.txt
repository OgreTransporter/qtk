################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################

################################################################################
# Qtk Library
################################################################################
set(
    QTK_LIBRARY_PUBLIC_HEADERS
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

set(
    QTK_LIBRARY_SOURCES
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

qt6_add_resources(QTK_LIBRARY_SOURCES "${QTK_RESOURCES}/resources.qrc")
qt_add_library(qtk-library STATIC)
target_sources(qtk-library PRIVATE ${QTK_LIBRARY_SOURCES})
target_sources(
    qtk-library PUBLIC
    FILE_SET HEADERS
      BASE_DIRS "${CMAKE_SOURCE_DIR}/src"
      FILES ${QTK_LIBRARY_PUBLIC_HEADERS}
)

if(QTK_DEBUG)
  target_compile_definitions(qtk-library PUBLIC QTK_DEBUG)
endif()

set_target_properties(
    qtk-library PROPERTIES
      WIN32_EXECUTABLE TRUE
      MACOSX_BUNDLE TRUE
      VERSION ${PROJECT_VERSION}
)

target_link_libraries(
    qtk-library PUBLIC
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
install(
    TARGETS qtk-library
    # Associate qtk-library target with qtk-export
    EXPORT qtk-export
    FILE_SET HEADERS DESTINATION include
    BUNDLE DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

## Install qtk-library to Qt Designer to support widget plugins.
install(
    TARGETS qtk-library
    BUNDLE DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
    LIBRARY DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
    ARCHIVE DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
    RUNTIME DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
)
