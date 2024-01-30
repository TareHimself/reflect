cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
include(${CMAKE_CURRENT_SOURCE_DIR}/utils.cmake)


project(reflect VERSION 1.0.0 DESCRIPTION "Basic vulkan rendering engine")

file(GLOB_RECURSE SOURCE_FILES "reflect/*.cpp" )

file(GLOB_RECURSE INCLUDE_FILES "reflect/*.hpp" )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})


add_executable(main main.cpp Test.hpp Test.reflect.hpp)
target_link_libraries(main reflect)