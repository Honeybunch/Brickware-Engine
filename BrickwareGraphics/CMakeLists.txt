file(GLOB BRICKWARE_GRAPHICS_HEADERS "include/*.hpp")
file(GLOB BRICKWARE_GRAPHICS_SOURCE "src/*.cpp")

#C++ 11
if(UNIX)
	set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")
endif(UNIX)

include_directories("include/")
include_directories("../BrickwareMath/include")
include_directories("../BrickwareUtils/include")
include_directories(SYSTEM)

add_library(BrickwareCore SHARED ${BRICKWARE_GRAPHICS_SOURCE} ${BRICKWARE_GRAPHICS_SOURCE})

#Need to link math library manually
if(UNIX)
	target_link_libraries(BrickwareCore m)
endif(UNIX)

#Link other Brickware libraries
target_link_libraries(BrickwareGraphics BrickwareMath)
target_link_libraries(BrickwareGraphics BrickwareUtils)

set_property(TARGET BrickwareGraphics PROPERTY FOLDER "lib")

install(TARGETS BrickwareGraphics LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin)
