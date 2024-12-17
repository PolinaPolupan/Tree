# Specify minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Add your custom algorithm library
add_library(image_lib INTERFACE)

add_definitions(-DGVDLL=1)

set(GRAPHVIZ_DIR "${CMAKE_SOURCE_DIR}/src/external/Graphviz-12.1.2-win64")

set(CGRAPH_LIB "${GRAPHVIZ_DIR}/lib/cgraph++.lib") # or .lib on Windows

find_library(CGRAPH_LIB cgraph REQUIRED)

file(GLOB GVC_LIBS "${GRAPHVIZ_DIR}/lib/*.lib")

target_link_libraries(image_lib INTERFACE ${GVC_LIBS} ${CGRAPH_LIB})

target_include_directories(image_lib INTERFACE 
"${GRAPHVIZ_DIR}/include/graphviz" 
${CMAKE_CURRENT_SOURCE_DIR}/include
 ${CMAKE_SOURCE_DIR}/src/tree/include
)