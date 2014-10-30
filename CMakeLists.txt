cmake_minimum_required (VERSION 2.8.11)
project (BetaProbePositioningTable)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(driver)
add_subdirectory(src)
