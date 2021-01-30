#include "e57Scan.h"
#include <numeric>

class ScanManager
{
    std::vector<std::shared_ptr<e57Scan>> m_scans;
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    std::string guidOfImageToShow;

public:
    ScanManager();
    int addScanFile(const std::string &filename);
    void showScanInfo() const;
    bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);
    void showImage()
    {
        return;
        if (!my_image_texture)
        {
            bool ret = LoadTextureFromFile("MyImage01.jpeg", &my_image_texture, &my_image_width, &my_image_height);
            IM_ASSERT(ret);
        }
        ImGui::Begin("OpenGL Texture Text");
        ImGui::Text("pointer = %p", my_image_texture);
        ImGui::Text("size = %d x %d", my_image_width, my_image_height);
        ImGui::Image((void *)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
        ImGui::End();
    }
};