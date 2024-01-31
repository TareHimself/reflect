cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 20)
project(Standalone)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" )
file(GLOB_RECURSE INCLUDE_FILES "src/*.hpp" )

function(BuildOnly CLONED_DIR BUILD_DEST LOCAL_BUILD_TYPE BUILD_FN)
    set(BUILD_DIR ${CLONED_DIR}/build/${LOCAL_BUILD_TYPE})
    message(STATUS "DIR ${BUILD_DIR}")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -S ${CLONED_DIR} -B ${BUILD_DIR}
  )

  execute_process(
    COMMAND ${CMAKE_COMMAND} --build ${BUILD_DIR} --config ${LOCAL_BUILD_TYPE}
  )

  execute_process(
    COMMAND ${CMAKE_COMMAND} --install ${BUILD_DIR} --prefix ${BUILD_DEST} --config ${LOCAL_BUILD_TYPE}
  )
endfunction()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

set(REFLECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/reflect)
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/reflect)
    BuildOnly(${CMAKE_CURRENT_SOURCE_DIR}/.. ${REFLECT_DIR} "Release" "")
    BuildOnly(${CMAKE_CURRENT_SOURCE_DIR}/.. ${REFLECT_DIR} "Debug" "")
endif()

list(APPEND CMAKE_PREFIX_PATH ${REFLECT_DIR}/lib/cmake)
find_package(reflect REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${REFLECT_DIR}/include)
target_link_libraries(${PROJECT_NAME} reflect::reflect)
add_executable(main src/main.cpp)
target_link_libraries(main reflect::reflect)