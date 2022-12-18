################################################################################
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
##                                                                            ##
## Project for working with OpenGL and Qt6 widgets                            ##
################################################################################

# Qtk Library
add_subdirectory(qtk)

# Qtk Application
if (QTK_BUILD_GUI)
  #TODO: Use this to test local builds?
#  SET(CMAKE_INSTALL_RPATH "${QTK_INSTALL_PREFIX}/lib")
  add_subdirectory(app)
endif()

# Install export
install(EXPORT qtk-export
    FILE QtkTargets.cmake
    NAMESPACE Qtk::
    CONFIGURATIONS Debug|Release
    DESTINATION ${QTK_INSTALL_PREFIX}/cmake
)
