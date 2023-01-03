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
    qtkwidget.cpp qtkwidget.h
    debugconsole.cpp debugconsole.ui
    toolbox.cpp toolbox.ui
    treeview.cpp treeview.ui
)
set(
    QTK_PLUGIN_LIBRARY_HEADERS
    debugconsole.h
    toolbox.h
    treeview.h
)
qt_add_library(qtk-plugin-library SHARED)
target_sources(
    qtk-plugin-library PRIVATE
    "${QTK_PLUGIN_LIBRARY_SOURCES}"
    "${QTK_PLUGIN_LIBRARY_HEADERS}"
)
target_link_libraries(qtk-plugin-library PUBLIC Qt6::UiPlugin qtk-library)

install(
    TARGETS qtk-plugin-library
    BUNDLE DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
)

install(
    TARGETS qtk-plugin-library
    BUNDLE DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
    LIBRARY DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
    RUNTIME DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
    ARCHIVE DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
)

################################################################################
# Qtk Widget Collection Plugin
################################################################################
# Create a Qt Designer plugin for a collection of widgets from our library.
qt_add_plugin(qtk-collection)
target_sources(
    qtk-collection PRIVATE
    widgetplugincollection.cpp widgetplugincollection.h
    widgetplugin.cpp widgetplugin.h
)
target_link_libraries(qtk-collection PUBLIC qtk-plugin-library)

install(
    TARGETS qtk-collection
    RUNTIME DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
    BUNDLE DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
    LIBRARY DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
)

################################################################################
# Final Qtk Application
################################################################################

set(
    QTK_APP_SOURCES
    examplescene.cpp examplescene.h
    qtkmainwindow.cpp qtkmainwindow.ui
    qtkmainwindow.h
    main.cpp
)
# TODO: Qtk library should not depend on these resources. (qtkapi.h)
#   Or should some resources be installed somewhere?
qt6_add_resources(QTK_APP_SOURCES "${QTK_RESOURCES}/resources.qrc")
configure_file(
    resources.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/resources.h"
    @ONLY
)

qt_add_executable(qtk-main ${QTK_APP_SOURCES})
target_link_libraries(qtk-main PRIVATE qtk-plugin-library)

set_target_properties(
    qtk-main PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

install(
    TARGETS qtk-main
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin/lib
)
