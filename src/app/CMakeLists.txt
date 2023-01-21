################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################

################################################################################
# Qtk Widget Library
################################################################################
# Create a library of widgets used to build Qtk GUI
set(
    QTK_PLUGIN_LIBRARY_SOURCES
    qtkwidget.cpp
    debugconsole.cpp debugconsole.ui
    toolbox.cpp toolbox.ui
    treeview.cpp treeview.ui
)
set(
    QTK_PLUGIN_LIBRARY_HEADERS
    qtkwidget.h
    debugconsole.h
    toolbox.h
    treeview.h
)
qt_add_library(qtk_plugin_library STATIC)
target_sources(
    qtk_plugin_library PRIVATE
    "${QTK_PLUGIN_LIBRARY_SOURCES}"
    "${QTK_PLUGIN_LIBRARY_HEADERS}"
)
target_link_libraries(qtk_plugin_library PUBLIC Qt6::UiPlugin qtk_library)

install(
    TARGETS qtk_plugin_library
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
)

# Install plugin library to Qt Creator for Designer plugin integration.
#install(
#    TARGETS qtk_plugin_library
#    EXCLUDE_FROM_ALL
#    LIBRARY DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
#    ARCHIVE DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
#    RUNTIME DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
#)

################################################################################
# Qtk Widget Collection Plugin
################################################################################
# Create a Qt Designer plugin for a collection of widgets from our library.
qt_add_plugin(qtk_collection STATIC)
target_sources(
    qtk_collection PRIVATE
    widgetplugincollection.cpp widgetplugincollection.h
    widgetplugin.cpp widgetplugin.h
)
target_link_libraries(qtk_collection PUBLIC qtk_plugin_library)

install(
    TARGETS qtk_collection
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
)

#install(
#    TARGETS qtk_collection
#    EXCLUDE_FROM_ALL
#    LIBRARY DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
#    ARCHIVE DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
#    RUNTIME DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
#)

################################################################################
# Final Qtk Application
################################################################################

set(
    QTK_APP_SOURCES
    examplescene.cpp examplescene.h
    qtkmainwindow.cpp qtkmainwindow.h qtkmainwindow.ui
    main.cpp
)

qt6_add_big_resources(QTK_APP_SOURCES "${QTK_RESOURCES}/resources.qrc")
configure_file(
    resources.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/resources.h"
    @ONLY
)

qt_add_executable(qtk_main ${QTK_APP_SOURCES})
target_link_libraries(qtk_main PRIVATE qtk_plugin_library)

set_target_properties(
    qtk_main PROPERTIES
      WIN32_EXECUTABLE TRUE
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_BUNDLE_NAME Qtk
      MACOSX_BUNDLE_ICON_FILE ${QTK_OSX_ICONS}
      MACOSX_BUNDLE_GUI_IDENTIFIER ${CMAKE_PROJECT_NAME}
      MACOSX_BUNDLE_INFO_STRING ${CMAKE_PROJECT_DESCRIPTION}
      MACOSX_BUNDLE_COPYRIGHT "All Content (c) 2023 Shaun Reed, all rights reserved"
      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
      MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

install(
    TARGETS qtk_main
    BUNDLE DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
)

qt_generate_deploy_app_script(
    TARGET qtk_main
    FILENAME_VARIABLE QTK_DEPLOY_SCRIPT
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${QTK_DEPLOY_SCRIPT})

if(WIN32)
  if(MSVC AND TARGET Qt6::qmake)
    set(VSUSER_FILE "${CMAKE_CURRENT_BINARY_DIR}/qtk_main.vcxproj.user")
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
