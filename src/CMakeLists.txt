# Include sub-projects.
add_subdirectory(tree)
add_subdirectory(image)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tree_exe)