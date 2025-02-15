cmake_minimum_required(VERSION 3.5)
project(neural-network)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES CACHE INTERNAL "")

include(vendors/raylib.cmake)
include(vendors/raylib-cpp.cmake)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    raylib
    raylib_cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE core)

include(core/core.cmake)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

install(
    TARGETS ${PROJECT_NAME}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)