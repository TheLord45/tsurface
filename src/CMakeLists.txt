cmake_minimum_required(VERSION 3.16)

project(tsurface VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_add_executable(tsurface
    main.cpp
    tsurface.ui
    tsurface.cpp
    tsurface.qrc
    tsurfacereader.cpp
    tsurfacereader.h
    terror.cpp
    terror.h
    tconfig.cpp
    tconfig.h
    tconfmain.cpp
    tconfmain.h
    tnewprojectdialog.cpp
    tnewprojectdialog.h
    tnewprojectdialog.ui
    tpaneltypes.h
    tpaneltypes.cpp
    tpagetree.h
    tpagetree.cpp
    tpagehandler.cpp
    tpagehandler.h
    tprojectproperties.cpp
    tprojectproperties.h
    tprojectproperties.ui
    tobjecthandler.cpp
    tobjecthandler.h
    tpopuplist.cpp
    tpopuplist.h
    tpopuplist.ui
    taddpagedialog.cpp
    taddpagedialog.h
    taddpagedialog.ui
    tfonts.cpp
    tfonts.h
    taddpopupdialog.cpp
    taddpopupdialog.h
    taddpopupdialog.ui
    borders.qrc
    tgraphics.h tgraphics.cpp
)

# Use the Widgets module from Qt 6.
target_link_libraries(tsurface PRIVATE Qt6::Widgets archive)

# Install the executable
install(TARGETS tsurface DESTINATION bin)
