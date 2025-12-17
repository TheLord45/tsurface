qt_add_executable(tsurface
    main.cpp
    tsurface.ui
    tsurface.cpp
    tsurfacereader.cpp
    tsurfacereader.h
    terror.cpp
    terror.h
    tconfig.h
    tconfig.cpp
    tconfmain.h
    tconfmain.cpp
)

# Use the Widgets module from Qt 6.
target_link_libraries(tsurface PRIVATE Qt6::Widgets archive)

# Install the executable
install(TARGETS tsurface DESTINATION bin)
