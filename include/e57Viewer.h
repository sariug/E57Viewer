#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ScanManager.h"

class e57Viewer
{
    int init();
    e57Viewer();
    e57Viewer(const e57Viewer&) = delete;
    e57Viewer(e57Viewer&&) = delete;
    e57Viewer& operator=(const e57Viewer&) = delete;
    e57Viewer& operator=(e57Viewer&&) = delete;

public:
    static e57Viewer& instance();
    void run();
    void importScan(std::string filename);
    void prepareShutdown();

private:
    static e57Viewer *m_instance;
    GLFWwindow *window;
    std::unique_ptr<ScanManager> m_manager;
};