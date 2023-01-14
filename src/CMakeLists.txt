################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################

# Qtk Library
add_subdirectory(qtk)

# Qtk Application
if (QTK_BUILD_GUI)
  add_subdirectory(app)
endif()

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/QtkConfigVersion.cmake"
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/templates/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QtkConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Qtk
)
install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/QtkConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/QtkConfigVersion.cmake"
    DESTINATION lib/cmake/Qtk
)
install(
    EXPORT qtk-export
    FILE QtkTargets.cmake
    NAMESPACE Qtk::
    DESTINATION lib/cmake/Qtk
)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VENDOR "Shaun Reed")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt OpenGL 3D graphics library.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/shaunrd0/qtk")
include(CPack)
