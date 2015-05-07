file(GLOB BRICKWARE_UTILS_HEADERS "include/BrickwareUtils/*.hpp")
file(GLOB BRICKWARE_UTILS_SOURCE "src/*.cpp")

#C++ 11
if(UNIX)
set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")
endif(UNIX)

include_directories("${CMAKE_SOURCE_DIR}/BrickwareUtils/include/")
include_directories(SYSTEM)

#Need to manually include GLFW and GLEW on Windows
if(WIN32)
	include_directories(${GLEW_INCLUDE})
	include_directories(${GLFW_INCLUDE})
endif(WIN32)

source_group("Headers" FILES ${BRICKWARE_UTILS_HEADERS})
add_library(BrickwareUtils SHARED ${BRICKWARE_UTILS_SOURCE} ${BRICKWARE_UTILS_HEADERS})

#Need to link some basic libraries
if(UNIX)
	target_link_libraries(BrickwareUtils m)
endif(UNIX)

#Link GL and GLEW for shader logging
if(GL_SUPPORT)
	if(UNIX)
		target_link_libraries(BrickwareUtils GL)
		target_link_libraries(BrickwareUtils GLEW)
	endif(UNIX)
	if(WIN32)
		target_link_libraries(BrickwareUtils opengl32.lib)
		target_link_libraries(BrickwareUtils ${GLEW_LIB}/glew32.lib)
	endif(WIN32)
endif(GL_SUPPORT)

set_property(TARGET BrickwareUtils PROPERTY FOLDER "lib")

install(TARGETS BrickwareUtils 
		LIBRARY DESTINATION ${PROJECT_BINARY_DIR}
		ARCHIVE DESTINATION ${PROJECT_BINARY_DIR})
