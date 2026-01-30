cmake_minimum_required(VERSION 3.16)

project(tsurface VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES en de gr)
qt6_add_translations(tsurface QM_FILES_OUTPUT_VARIABLE qm_files)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Core Gui Network)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Network)

if(${QT_VERSION_MAJOR} LESS 6)
    message(FATAL_ERROR "Qt with version ${QT_VERSION} is not supported!")
endif()

find_package(OpenSSL REQUIRED)
find_package(LibArchive REQUIRED)
find_package(Fontconfig REQUIRED)

if(APPLE)
    link_directories("/opt/homebrew/Cellar/libarchive/3.8.5/lib")
    include_directories("/opt/homebrew/Cellar/libarchive/3.8.5/include")
endif(APPLE)

qt_add_executable(tsurface
    main.cpp
    tsurface.ui
    tsurface.cpp
    tsurface.qrc
    borders.qrc
    sliders.qrc
    fonts.qrc
    sounds.qrc
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
    tgraphics.cpp
    tgraphics.h
    tsurfacewriter.cpp
    tsurfacewriter.h
    tmisc.h
    tmisc.cpp
    tresourcedialog.cpp
    tresourcedialog.h
    tresourcedialog.ui
    tmaps.cpp
    tmaps.h
    trenamefiledialog.cpp
    trenamefiledialog.h
    trenamefiledialog.ui
    tdynamicimagedialog.cpp
    tdynamicimagedialog.h
    tdynamicimagedialog.ui
    tdynamicdatadialog.cpp
    tdynamicdatadialog.h
    tdynamicdatadialog.ui
    tdatamapdialog.cpp
    tdatamapdialog.h
    tdatamapdialog.ui
    ttextboxdialog.cpp
    ttextboxdialog.h
    ttextboxdialog.ui
    tcharactermapdialog.cpp
    tcharactermapdialog.h
    tcharacterwidget.cpp
    tcharacterwidget.h
    tdownloader.cpp
    tdownloader.h
    tpropertiesgeneral.cpp
    tpropertiesgeneral.h
    tpropertiesprogramming.cpp
    tpropertiesprogramming.h
    tpropertiesstates.cpp
    tpropertiesstates.h
    tworkspacehandler.cpp
    tworkspacehandler.h
    ttexteditdialog.cpp
    ttexteditdialog.h
    ttexteditdialog.ui
    tcrypt.cpp
    tcrypt.h
    base64.cpp
    base64.h
    tvalidateport.cpp
    tvalidateport.h
    tcanvaswidget.cpp
    tcanvaswidget.h
    tresizablewidget.cpp
    tresizablewidget.h
    tbitmapdialog.cpp
    tbitmapdialog.h
    tbitmapdialog.ui
    telementwidgetcombo.cpp
    telementwidgetcombo.h
    tbitmapselectdialog.cpp
    tbitmapselectdialog.h
    tbitmapselectdialog.ui
    telementbitmapselector.cpp
    telementbitmapselector.h
)

# Use the Widgets module from Qt 6.
target_link_libraries(tsurface PRIVATE Qt6::Core Qt6::Widgets Qt6::Network archive fontconfig ssl crypto)

# Install the executable
install(TARGETS tsurface DESTINATION bin)
install(FILES ${qm_files} DESTINATION /usr/share/tsurface)
