qt_add_executable(tsurface
    main.cpp
    tsurface.ui
    tsurface.cpp
)

# Use the Widgets module from Qt 6.
target_link_libraries(tsurface PRIVATE Qt6::Widgets)

# Install the executable
install(TARGETS tsurface DESTINATION bin)
