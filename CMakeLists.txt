cmake_minimum_required(VERSION 2.8)
project(House_in_the_Forest)

# Before CMake 3.1
add_compile_options(-std=c++11)

# After CMake 3.1
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCES 	src/main.cpp
		src/model.cpp
		src/objectLoader.cpp
		src/texture.cpp
		src/object.cpp
		src/heightmap.cpp
		src/camera.cpp
		src/sky.cpp
		src/forest.cpp
		src/environment.cpp
		src/rain.cpp
		src/dynamicFog.cpp
		src/light.cpp
		src/sphere.cpp
		src/tdogl/Program.cpp
		src/tdogl/Program.h
		src/tdogl/Shader.cpp
		src/tdogl/Shader.h
		src/phong.fs
		src/phong.vs
		src/unifColor-Phong.fs
		src/unifColor-Phong.vs

		include/model.hpp
		include/objectLoader.hpp
		include/texture.hpp
		include/object.hpp
		include/heightmap.hpp
		include/camera.hpp
		include/sky.hpp
		include/forest.hpp
		include/environment.hpp
		include/rain.hpp
		include/light.hpp
		include/sphere.hpp
		)

add_executable(House_in_the_Forest ${SOURCES})

# Find SOIL
FIND_PATH( SOIL_INCLUDE_DIRS SOIL.h PATH_SUFFIXES SOIL )
FIND_LIBRARY( SOIL_LIBRARIES SOIL)

# Find FreeGlut
FIND_PATH(FREEGLUT_INCLUDE_DIRS NAMES GL/freeglut.h)
FIND_LIBRARY(FREEGLUT_LIBRARY NAMES freeglut)

include_directories( ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}  ${FREEGLUT_INCLUDE_DIRS} /include)

target_link_libraries(House_in_the_Forest ${OPENGL_LIBRARIES} ${FREEGLUT_LIBRARY} ${SOIL_LIBRARIES} ${GLEW_LIBRARIES}) 
