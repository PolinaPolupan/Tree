
add_library(tree_lib
INTERFACE
"include/TreeBase.h" 
"include/TreeNode.h" 
"include/Iterator.h" 
"include/IteratorReversed.h" 
"include/IteratorBase.h" 
"include/Obst.h"
)

target_compile_features(tree_lib INTERFACE cxx_std_17)

target_include_directories(tree_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Define the main executable
add_executable(tree_exe main.cpp)

target_compile_features(tree_exe PRIVATE cxx_std_17)

# Link the graph library to the main executable
target_link_libraries(tree_exe PRIVATE tree_lib image_lib)

target_include_directories(tree_exe PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/image/include
)

set(GRAPHVIZ_DIR "${CMAKE_SOURCE_DIR}/src/external/Graphviz-12.1.2-win64")

add_custom_command(TARGET tree_exe POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_directory # which executes "cmake - E copy_if_different..."
        "${GRAPHVIZ_DIR}/bin"    
        $<TARGET_FILE_DIR:tree_exe>)             


target_compile_definitions(tree_exe PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}")