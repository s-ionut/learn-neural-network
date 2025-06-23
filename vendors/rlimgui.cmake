set(LIB_NAME rlimgui)

include(FetchContent)

FetchContent_Declare(
    rlimgui_external
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(rlimgui_external)

add_library(${LIB_NAME}
    ${rlimgui_external_SOURCE_DIR}/rlImGui.cpp
)

target_link_libraries(${LIB_NAME} 
    PUBLIC 
    raylib
    imgui
)

target_include_directories(${LIB_NAME} 
    PUBLIC 
    ${rlimgui_external_SOURCE_DIR}
    ${imgui_external_SOURCE_DIR}
)