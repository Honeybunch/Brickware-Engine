file(GLOB BRICKWARE_CORE_HEADERS "include/*/*.hpp")
file(GLOB BRICKWARE_CORE_SOURCE "src/*.cpp")

#C++ 11
if(UNIX)
	set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")
endif(UNIX)

include_directories("${CMAKE_SOURCE_DIR}/BrickwareCore/include/")
include_directories("${CMAKE_SOURCE_DIR}/BrickwareMath/include")
include_directories("${CMAKE_SOURCE_DIR}/BrickwareUtils/include")
include_directories("${CMAKE_SOURCE_DIR}/BrickwareGraphics/include")
include_directories(SYSTEM)

#Need to manually include GLFW and GLEW on Windows
if(WIN32)
	include_directories(${GLEW_INCLUDE})
	include_directories(${GLFW_INCLUDE})
endif(WIN32)

source_group("Headers" FILES ${BRICKWARE_CORE_HEADERS})
add_library(BrickwareCore SHARED ${BRICKWARE_CORE_SOURCE} ${BRICKWARE_CORE_HEADERS})

#Need to link math library manually
if(UNIX)
	target_link_libraries(BrickwareCore m)
endif(UNIX)

#Link GL, GLEW and GLFW
if(GL_SUPPORT)
	if(UNIX)
		target_link_libraries(BrickwareCore GL)
		target_link_libraries(BrickwareCore GLEW)
		target_link_libraries(BrickwareCore glfw)
	endif(UNIX)

	if(WIN32)
		target_link_libraries(BrickwareCore opengl32.lib)
		target_link_libraries(BrickwareCore ${GLEW_LIB}/glew32.lib)
		target_link_libraries(BrickwareCore ${GLFW_LIB}/glfw3dll.lib)
	endif(WIN32)
endif(GL_SUPPORT)

if(D3D_SUPPORT)
	target_link_libraries(BrickwareCore d3d11.lib)
	target_link_libraries(BrickwareCore dxguid.lib)
	target_link_libraries(BrickwareCore d3dcompiler.lib)
endif(D3D_SUPPORT)

#Link other Brickware libraries
target_link_libraries(BrickwareCore BrickwareMath)
target_link_libraries(BrickwareCore BrickwareUtils)
target_link_libraries(BrickwareCore BrickwareGraphics)

set_property(TARGET BrickwareCore PROPERTY FOLDER "lib")

install(TARGETS BrickwareCore 
		LIBRARY DESTINATION ${PROJECT_BINARY_DIR}
		ARCHIVE DESTINATION ${PROJECT_BINARY_DIR})
