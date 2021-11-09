# Setup project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(OpenGL_Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(X11 REQUIRED)

# add include dir
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${X11_INCLUDE_DIR})

# add libraries
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${X11_LIBRARIES})

# add sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

# copy shaders to build dir
message(STATUS "Copying shaders to build dir")
SET(shader_dir ${PROJECT_SOURCE_DIR}/shaders)
file(GLOB shaders RELATIVE ${shader_dir} ${shader_dir}/*.glsl)
foreach(shader ${shaders})
  set(srcShaderPath ${shader_dir}/${shader})
  if(NOT IS_DIRECTORY ${srcShaderPath})
    message(STATUS "  Copying ${shader}")
    configure_file(
      ${srcShaderPath}
      shaders/${shader}
      COPYONLY)
  endif(NOT IS_DIRECTORY ${srcShaderPath})
endforeach(shader)

# add executable
add_executable(generator generator.cpp ${sources} ${headers})

# add libraries
set(GLFW_LIBS X11 rt m dl pthread)
target_link_libraries(generator glfw3 ${GLFW_LIBS})
