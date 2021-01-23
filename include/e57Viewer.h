#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>        
#include <GLFW/glfw3.h>

#include <memory>
class e57Viewer
{
    GLFWwindow* window;
    int init();
    void prepareShutdown();
    public: 
        e57Viewer();
        void run();
};