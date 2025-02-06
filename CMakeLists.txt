cmake_minimum_required(VERSION 3.19)
project(Pokemart LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(Pokemart
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h

    collapsiblesidebar.h collapsiblesidebar.cpp
    resources.qrc
    productcard.h productcard.cpp
    flowlayout.h flowlayout.cpp
    logindialog.h logindialog.cpp
    databasemanager.h databasemanager.cpp
    paymentdialog.h paymentdialog.cpp
    sessionmanager.h sessionmanager.cpp
)

target_link_libraries(Pokemart
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
)

# Ruta del archivo de base de datos en la raíz del proyecto
set(DB_SOURCE_PATH ${CMAKE_SOURCE_DIR}/pokemart.db)

# Ruta del archivo de base de datos en el directorio de construcción
set(DB_DEST_PATH ${CMAKE_BINARY_DIR}/pokemart.db)

# Copiar el archivo de base de datos al directorio de compilación
add_custom_command(TARGET Pokemart POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DB_SOURCE_PATH} ${DB_DEST_PATH}
    COMMENT "Copiando la base de datos a la carpeta de construcción"
)

include(GNUInstallDirs)

install(TARGETS Pokemart
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Copiar la base de datos durante la instalación
install(FILES ${DB_SOURCE_PATH} DESTINATION ${CMAKE_INSTALL_BINDIR})

qt_generate_deploy_app_script(
    TARGET Pokemart
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
