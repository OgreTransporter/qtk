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
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/templates/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)
install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)
install(
    EXPORT qtk_export
      FILE ${PROJECT_NAME}Targets.cmake
      NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VENDOR "Shaun Reed")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt OpenGL 3D graphics library.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/shaunrd0/qtk")
set(CPACK_SOURCE_IGNORE_FILES build*;install;\.git;\.github;\.idea)
set(CPACK_PACKAGE_DIRECTORY packages/)
set(CPACK_PACKAGE_CONTACT "shaunreed.com")
#set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/resources/icon.png")
set(CPACK_THREADS 0)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Qtk")
set(CPACK_COMPONENTS_ALL Unspecified)
# Debian
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CPACK_PACKAGE_HOMEPAGE_URL})
set(CPACK_INSTALL_SCRIPTS ${QTK_DEPLOY_SCRIPT})

# OSX
set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
set(CPACK_BUNDLE_PLIST $<TARGET_BUNDLE_CONTENT_DIR:qtk_main>/Info.plist)
set(CPACK_BUNDLE_ICON ${QTK_OSX_ICONS})
if(WIN32)
  set(CPACK_SOURCE_GENERATOR ZIP)
else()
  set(CPACK_SOURCE_GENERATOR TGZ)
endif()

include(CPack)
