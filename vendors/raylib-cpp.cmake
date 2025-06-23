# raylib-cpp
find_package(raylib_cpp QUIET)

if(NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()