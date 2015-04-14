cmake_minimum_required (VERSION 2.6)

project(Brickware-Engine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(BrickwareMath)
add_subdirectory(BrickwareUtils)
add_subdirectory(BrickwareGraphics)
add_subdirectory(BrickwareCore)
add_subdirectory(BrickwareTest)
