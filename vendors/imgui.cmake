set(LIB_NAME imgui)

include(FetchContent)

FetchContent_Declare(
    imgui_external
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(imgui_external)

add_library(${LIB_NAME}
    ${imgui_external_SOURCE_DIR}/imgui.cpp
    ${imgui_external_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_external_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_external_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_external_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(${LIB_NAME} PRIVATE glfw)

target_include_directories(${LIB_NAME} PUBLIC ${imgui_external_SOURCE_DIR})