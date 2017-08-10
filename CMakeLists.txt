#
# Project Properties
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(particles)
SET(APPLICATION_NAME "particles")
SET(APPLICATION_CODENAME "${PROJECT_NAME}")
SET(APPLICATION_COPYRIGHT_YEARS "2017")
SET(APPLICATION_VERSION_MAJOR 4)
SET(APPLICATION_VERSION_MINOR 0)
SET(APPLICATION_VERSION_PATCH 0)
SET(APPLICATION_VERSION_TYPE "RELEASE")
SET(APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}-${APPLICATION_VERSION_TYPE}")
SET(APPLICATION_ID "${PROJECT_NAME}")

#
# Debugging Options
#
SET(CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release

#
# Project Output Paths
#
SET(MAINFOLDER ${PROJECT_SOURCE_DIR})
SET(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")

#
# Project Search Paths
#
LIST(APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}")
INCLUDE_DIRECTORIES("${MAINFOLDER}/include")

#
# Set compiler warnings
#
SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -Wno-deprecated-declarations -Wno-pessimizing-move")

if(WIN32)
else()
    if(APPLE)
    elseif(UNIX)
        add_definitions(-DGLEW_EGL)
    endif()
endif()

#
# glfw
#
OPTION(GLFW_BUILD_EXAMPLES  "Build the GLFW example programs"   OFF)
OPTION(GLFW_BUILD_TESTS     "Build the GLFW test programs"      OFF)
OPTION(GLFW_BUILD_DOCS      "Build the GLFW documentation"      OFF)
OPTION(GLFW_INSTALL         "Generate installation target"      OFF)

ADD_SUBDIRECTORY(lib/glfw)

SET(GLFW_INCLUDE_PATH "${MAINFOLDER}/lib/glfw/include")

#
# glew
#
OPTION(glew-cmake_BUILD_SHARED "Build the shared glew library" OFF)
ADD_SUBDIRECTORY(lib/glew)

SET(GLEW_INCLUDE_PATH "${MAINFOLDER}/lib/glew/include")

#
# glm
#
SET(GLM_INCLUDE_PATH "${MAINFOLDER}/lib/glm")

#
# assimp
#
SET(ASSIMP_INCLUDE_PATH "${MAINFOLDER}/lib/assimp/include")
ADD_SUBDIRECTORY(lib/assimp)

#
# Add project sources
#
ADD_SUBDIRECTORY(src)
