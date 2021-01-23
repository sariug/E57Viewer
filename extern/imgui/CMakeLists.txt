
add_library(imgui
    # Main Imgui files
    imgui.cpp 
    imgui_draw.cpp
    imgui_tables.cpp 
    imgui_widgets.cpp
    imgui_demo.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp)

# Make SDL2 available to Imgui
target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})
# imgui/backends/ contains the SDL implementation
target_include_directories(imgui PUBLIC .)
