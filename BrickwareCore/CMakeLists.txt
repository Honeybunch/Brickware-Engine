file(GLOB BRICKWARE_CORE_HEADERS "include/*.hpp")
file(GLOB BRICKWARE_CORE_SOURCE "src/*.cpp")

#C++ 11
set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")

include_directories("include/")
include_directories("../BrickwareMath/include")
include_directories(SYSTEM)

add_library(BrickwareCore SHARED ${BRICKWARE_CORE_SOURCE} ${BRICKWARE_CORE_SOURCE})

#Need to link math library manually
if(UNIX)
	target_link_libraries(BrickwareCore m)
endif(UNIX)

#Link math library
target_link_libraries(BrickwareCore BrickwareMath)

set_property(TARGET BrickwareCore PROPERTY FOLDER "lib")

install(TARGETS BrickwareCore LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin)
