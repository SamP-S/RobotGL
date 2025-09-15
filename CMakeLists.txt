cmake_minimum_required(VERSION 3.20)

project(Robo-Sim VERSION 1.0.0 LANGUAGES CXX)

# set C++ standard globals
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# default to Debug build type if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug, Release, etc.)" FORCE)
endif()

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

# set include/src folders/files
set(INCLUDE_DIRS  "include")
file(GLOB_RECURSE SRC_FILES "src/**.cpp")
message(STATUS "INCLUDE_DIRS: " ${INCLUDE_DIRS})
message(STATUS "SRC_FILES: ${SRC_FILES}")

# runtime
add_executable(robots "${SRC_FILES}")
target_include_directories(robots PUBLIC ${INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
target_include_directories(robots PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(robots PRIVATE ${OPENGL_LIBRARIES})
message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIRS}")
message(STATUS "OpenGL libraries are ${OPENGL_LIBRARIES}")

find_package(SDL2 REQUIRED)
target_include_directories(robots PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(robots PRIVATE ${SDL2_LIBRARIES})
message(STATUS "SDL2 included at ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries are ${SDL2_LIBRARIES}")

find_package(GLEW REQUIRED)
target_include_directories(robots PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(robots PRIVATE ${GLEW_LIBRARIES})
message(STATUS "GLEW included at ${GLEW_INCLUDE_DIRS}")
message(STATUS "GLEW libraries are ${GLEW_LIBRARIES}")


