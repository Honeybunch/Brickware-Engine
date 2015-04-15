file(GLOB BRICKWARE_GRAPHICS_HEADERS "include/*/*.hpp")
file(GLOB BRICKWARE_GRAPHICS_SOURCE "src/*.cpp")

#C++ 11
if(UNIX)
	set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")
endif(UNIX)

include_directories("include/")
include_directories("../BrickwareMath/include")
include_directories("../BrickwareUtils/include")
include_directories(SYSTEM)

#Need to manually include GLFW and GLEW on Windows
if(WIN32)
	include_directories(${GLEW_INCLUDE})
	include_directories(${GLFW_INCLUDE})
endif(WIN32)

source_group("Headers" FILES ${BRICKWARE_GRAPHICS_HEADERS})
add_library(BrickwareGraphics SHARED ${BRICKWARE_GRAPHICS_SOURCE} ${BRICKWARE_GRAPHICS_HEADERS})

#Need to link math library manually
if(UNIX)
	target_link_libraries(BrickwareGraphics m)
endif(UNIX)

#Link GL, GLEW and GLFW
if(GL_SUPPORT)

	if(UNIX)
		target_link_libraries(BrickwareGraphics GL)
		target_link_libraries(BrickwareGraphics GLEW)
		target_link_libraries(BrickwareGraphics glfw)
	endif(UNIX)

	if(WIN32)
		target_link_libraries(BrickwareGraphics opengl32.lib)
		target_link_libraries(BrickwareGraphics ${GLEW_LIB}/glew32.lib)
		target_link_libraries(BrickwareGraphics ${GLFW_LIB}/glfw3dll.lib)
	endif(WIN32)

endif(GL_SUPPORT)

#Link other Brickware libraries
target_link_libraries(BrickwareGraphics BrickwareMath)
target_link_libraries(BrickwareGraphics BrickwareUtils)

set_property(TARGET BrickwareGraphics PROPERTY FOLDER "lib")

install(TARGETS BrickwareGraphics 
		LIBRARY DESTINATION ${PROJECT_BINARY_DIR}
		ARCHIVE DESTINATION ${PROJECT_BINARY_DIR})