file(GLOB BRICKWARE_MATH_HEADERS "include/BrickwareMath/*.hpp")
file(GLOB BRICKWARE_MATH_SOURCE "src/*.cpp")

#C++ 11
if(UNIX)
	set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")
endif(UNIX)

include_directories("include")
include_directories(SYSTEM)

source_group("Headers" FILES ${BRICKWARE_MATH_HEADERS})
add_library(BrickwareMath SHARED ${BRICKWARE_MATH_SOURCE} ${BRICKWARE_MATH_HEADERS})

#Need to link some basic libraries
if(UNIX)
	target_link_libraries(BrickwareMath m)
endif(UNIX)

set_property(TARGET BrickwareMath PROPERTY FOLDER "lib")

install(TARGETS BrickwareMath 
		LIBRARY DESTINATION ${PROJECT_BINARY_DIR}
		ARCHIVE DESTINATION ${PROJECT_BINARY_DIR})
