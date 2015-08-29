file(GLOB BRICKWARE_TEST_HEADERS "include/*.hpp")
file(GLOB BRICKWARE_TEST_SOURCE "src/*.cpp")

#C++ 11
if(UNIX)
	set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")
endif(UNIX)

include_directories(	"${CMAKE_SOURCE_DIR}/BrickwareTest/include/"
			"${CMAKE_SOURCE_DIR}/BrickwareMath/include"
			"${CMAKE_SOURCE_DIR}/BrickwareUtils/include"
			"${CMAKE_SOURCE_DIR}/BrickwareGraphics/include"
			"${CMAKE_SOURCE_DIR}/BrickwareCore/include")
include_directories(SYSTEM)

#Need to manually include GLFW and GLEW on Windows
if(WIN32)
	include_directories(${GLEW_INCLUDE})
	include_directories(${GLFW_INCLUDE})
endif(WIN32)

source_group("Headers" FILES ${BRICKWARE_TEST_HEADERS})
add_executable(BrickwareTest ${BRICKWARE_TEST_SOURCE} ${BRICKWARE_TEST_HEADERS})

#Need to link math library manually
if(UNIX)
	target_link_libraries(BrickwareTest m)
endif(UNIX)

#Link GL, GLEW and GLFW
if(GL_SUPPORT)

	if(UNIX)
		target_link_libraries(BrickwareTest GL)
		target_link_libraries(BrickwareTest GLEW)
		target_link_libraries(BrickwareTest glfw)
	endif(UNIX)

	if(WIN32)
		target_link_libraries(BrickwareTest opengl32.lib)
		target_link_libraries(BrickwareTest ${GLEW_LIB}/glew32.lib)
		target_link_libraries(BrickwareTest ${GLFW_LIB}/glfw3dll.lib)
	endif(WIN32)

endif(GL_SUPPORT)

if(D3D_SUPPORT)
	target_link_libraries(BrickwareTest d3d11.lib)
	target_link_libraries(BrickwareTest dxguid.lib)
	target_link_libraries(BrickwareTest d3dcompiler.lib)
endif(D3D_SUPPORT)

#Link other Brickware libraries
target_link_libraries(BrickwareTest BrickwareMath)
target_link_libraries(BrickwareTest BrickwareUtils)
target_link_libraries(BrickwareTest BrickwareGraphics)
target_link_libraries(BrickwareTest BrickwareCore)

set_property(TARGET BrickwareTest PROPERTY FOLDER "lib")

#Copy OpenGL Shared Libs
if(WIN32 AND GL_SUPPORT)
	add_custom_command(TARGET BrickwareTest POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${GLEW_LIB}/glew32.dll ${BUILD_DIR})
	add_custom_command(TARGET BrickwareTest POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${GLFW_LIB}/glfw3.dll ${BUILD_DIR})
endif(WIN32 AND GL_SUPPORT)

#Copy Shaders
add_custom_command(TARGET BrickwareTest POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Shaders ${BUILD_DIR}/Shaders)

install(TARGETS BrickwareTest DESTINATION ${PROJECT_BINARY_DIR})
