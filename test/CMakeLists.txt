cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 20)
project(ReflectTest)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" )
file(GLOB_RECURSE INCLUDE_FILES "src/*.hpp" )


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})


# target_include_directories(${PROJECT_NAME} PUBLIC ${REFLECT_DIR}/include)
# target_link_libraries(${PROJECT_NAME} reflect::reflect)
# add_executable(main src/main.cpp ${SOURCE_FILES} ${INCLUDE_FILES})
# target_link_libraries(main reflect::reflect)

add_subdirectory(../ reflect)
add_executable(main src/main.cpp ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(main reflect)
target_link_libraries(${PROJECT_NAME} reflect)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../)
add_custom_command ( TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy#_if_different
    $<TARGET_FILE:reflect> $<TARGET_FILE_DIR:main>
)