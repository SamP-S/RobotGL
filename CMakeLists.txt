cmake_minimum_required(VERSION 3.8)

project(Robo-Sim VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# set opengl config
if (POLICY CMP0072)
  cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

# git pull submodules
find_package(Git REQUIRED)
if (NOT EXISTS ${LIBTFD_DIR}/CMakeLists.txt)
    execute_process(COMMAND ${GIT_EXECUTABLE}
        submodule update --init --recursive
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()

# 
set(INCLUDE_DIRS  "include")
file(GLOB_RECURSE SRC_FILES "src/**.cpp")
message(STATUS "INCLUDE_DIRS: " ${INCLUDE_DIRS})
message(STATUS "SRC_FILES: ${SRC_FILES}")


# runtime
add_executable(robots "src/main.cpp")
target_include_directories(robots PUBLIC ${INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
target_include_directories(robots PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(robots PUBLIC ${OPENGL_LIBRARIES})
message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIRS}")
message(STATUS "OpenGL libraries are ${OPENGL_LIBRARIES}")

find_package(SDL2 REQUIRED)
target_include_directories(robots PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(robots PUBLIC ${SDL2_LIBRARIES})
message(STATUS "SDL2 included at ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries are ${SDL2_LIBRARIES}")

find_package(GLEW REQUIRED)
target_include_directories(robots PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(robots PUBLIC ${GLEW_LIBRARIES})
message(STATUS "GLEW included at ${GLEW_INCLUDE_DIRS}")
message(STATUS "GLEW libraries are ${GLEW_LIBRARIES}")


