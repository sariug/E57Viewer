#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>        
#include <GLFW/glfw3.h>
#include "ScanManager.h"

class e57Viewer
{
    int init();
    void prepareShutdown();

    public: 
        e57Viewer();
        ~e57Viewer();
        void run();

    private:
    GLFWwindow* window;
    std::unique_ptr<ScanManager> m_manager;
};