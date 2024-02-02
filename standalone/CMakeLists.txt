cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 20)
project(Standalone)
include(../utils.cmake)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" )
file(GLOB_RECURSE INCLUDE_FILES "src/*.hpp" )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

set(REFLECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/reflect)
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/reflect)
    BuildOnly(${CMAKE_CURRENT_SOURCE_DIR}/.. ${REFLECT_DIR} "Release" "")
    BuildOnly(${CMAKE_CURRENT_SOURCE_DIR}/.. ${REFLECT_DIR} "Debug" "")
endif()


# Argparse
macro(GetArgparse VERSION)
  set(BUILD_DEST ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/argparse)
  function(BuildArgparse B_TYPE B_SRC B_DEST)
    execute_process(
      COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=${B_TYPE} -DARGPARSE_BUILD_TESTS=OFF -S ${B_SRC} -B ${B_DEST}
    )
  endfunction()

  BuildThirdPartyDep(argparse https://github.com/p-ranav/argparse ${VERSION} RESULT_DIR "" "BuildArgparse")
  list(APPEND CMAKE_PREFIX_PATH ${RESULT_DIR}/lib/cmake)
  target_include_directories(${PROJECT_NAME} PUBLIC ${RESULT_DIR}/include)
endmacro()

GetArgparse(v3.0)

add_executable(main src/main.cpp)

find_package(argparse REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${REFLECT_DIR}/include)

# target_link_libraries(${PROJECT_NAME} argparse::argparse)
target_include_directories(main PUBLIC ${REFLECT_DIR}/include)
target_link_libraries(main argparse::argparse)

list(APPEND CMAKE_PREFIX_PATH ${REFLECT_DIR}/lib/cmake)

find_package(reflect REQUIRED)
# target_include_directories(${PROJECT_NAME} PUBLIC ${REFLECT_DIR}/include)
# target_link_libraries(${PROJECT_NAME} reflect::reflect)
target_include_directories(main PUBLIC ${REFLECT_DIR}/include)
target_link_libraries(main reflect::reflect)

add_custom_command ( TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:main> $<TARGET_FILE_DIR:main>
)