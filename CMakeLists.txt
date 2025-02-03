cmake_minimum_required(VERSION 3.19)
project(Prueba2 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(Prueba2
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    collapsiblesidebar.h collapsiblesidebar.cpp
    resources.qrc
    productcard.h productcard.cpp
    flowlayout.h flowlayout.cpp
    logindialog.h logindialog.cpp
    databasemanager.h databasemanager.cpp
    paymentdialog.h paymentdialog.cpp

)

target_link_libraries(Prueba2
    PRIVATE
    Qt6::Core
            Qt6::Widgets
            Qt6::Sql
)

include(GNUInstallDirs)

install(TARGETS Prueba2
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Prueba2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
