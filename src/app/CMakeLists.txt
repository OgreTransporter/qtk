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
    qtkmainwindow.cpp qtkmainwindow.ui
)
set(
    QTK_PLUGIN_LIBRARY_HEADERS
    qtkmainwindow.h
    qtkwidget.h
    debugconsole.h
    toolbox.h
    treeview.h
)
qt_add_library(qtk-plugin-library STATIC)
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
    main.cpp
)

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

qt_generate_deploy_app_script(
    TARGET qtk-main
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

if(WIN32)
  get_target_property(_qt6_qmake_location Qt6::qmake IMPORTED_LOCATION)
  execute_process(
      COMMAND "${_qt6_qmake_location}" -query QT_INSTALL_PREFIX
      RESULT_VARIABLE return_code
      OUTPUT_VARIABLE qt6_install_prefix
      OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  file(TO_NATIVE_PATH "${qt6_install_prefix}/bin" qt6_install_prefix)

  if(TARGET Qt6::windeployqt)
    add_custom_command(
        TARGET qtk-main
        POST_BUILD
        COMMAND set PATH=%PATH%$<SEMICOLON>${qt6_install_prefix}
        COMMAND Qt6::windeployqt --dir "${CMAKE_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:qtk-main>/$<TARGET_FILE_NAME:qtk-main>"
    )
    install(DIRECTORY "${CMAKE_BINARY_DIR}/windeployqt/" DESTINATION bin)
  endif()

  if(MSVC AND TARGET Qt6::qmake)
    set(VSUSER_FILE "${CMAKE_CURRENT_BINARY_DIR}/qtk-main.vcxproj.user")
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
