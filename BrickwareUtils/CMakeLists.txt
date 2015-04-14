file(GLOB BRICKWARE_UTILS_HEADERS "include/*.hpp")
file(GLOB BRICKWARE_UTILS_SOURCE "src/*.cpp")

#C++ 11
set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")

include_directories(include/)
include_directories(SYSTEM)

add_library(BrickwareUtils SHARED ${BRICKWARE_UTILS_HEADERS} ${BRICKWARE_UTILS_SOURCE})

#Need to link some basic libraries
if(UNIX)
	target_link_libraries(BrickwareUtils m)
endif(UNIX)

set_property(TARGET BrickwareUtils PROPERTY FOLDER "lib")

install(TARGETS BrickwareUtils LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin)
