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
    EXPORT qtk_export
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
set(CPACK_SOURCE_IGNORE_FILES build;install;\.git;\.github;\.idea)
set(CPACK_PACKAGE_DIRECTORY packages/)
set(CPACK_PACKAGE_CONTACT "shaunreed.com")
# TODO: This works for debian packages but the script installs to the wrong dir.
set(CPACK_INSTALL_SCRIPTS ${QTK_DEPLOY_SCRIPT})

# OSX
set(CPACK_BUNDLE_NAME Qtk)
set(CPACK_BUNDLE_PLIST ${CMAKE_BINARY_DIR}/bin/qtk_main.app/Contents/Info.plist)
set(CPACK_BUNDLE_ICON ${QTK_OSX_ICONS})
if(WIN32)
  set(CPACK_SOURCE_GENERATOR ZIP)
else()
  set(CPACK_SOURCE_GENERATOR TGZ)
endif()
include(CPack)
