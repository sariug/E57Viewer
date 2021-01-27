#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "E57SimpleReader.h"
#include <GL/glew.h>
#include "imgui.h"

struct ScanPt
{
    float x = 0.0, y = 0.0, z = 0.0, i = 0.0;
    uint8_t r = 0, g = 0, b = 0;
};
struct ScanData
{
    std::int64_t nCol, nRow, nTotalPoints, nGroupSize, nPointsPerGroup;
    bool colIndex = true;
    std::vector<ScanPt> pointCloud;
    void setPt(int col, int row, ScanPt &pt)
    {
        pointCloud.at(col * nRow + row) = pt;
    }
    ScanPt &getPt(int col, int row)
    {
        std::cout<<"getting scan Point"<<std::endl;
        return pointCloud.at(col * nRow + row);
    }
    bool showPointCloud = false;
    e57::Data3D header;

    GLuint scanColoredTex;

    // Simple helper function to load an image into a OpenGL texture with common settings
    bool createScanImage()
    {
        unsigned char *image_data = new unsigned char[nCol * nRow * 4]; //stbi_load(filename, &image_width, &image_height, NULL, 4);
        int c = 0;
        for (int i = 0; i < nCol; i++)
            for (int j = 0; j < nRow; j++)
            {
                const auto pt = getPt(i, j);
                image_data[c++] = pt.i*255;
                image_data[c++] = pt.i*255;
                image_data[c++] = pt.i*255;
                image_data[c++] = 255;
                if(pt.i>0)
                std::cout<<image_data[c-3]<<pt.i<<pt.i<<std::endl;
            }
        if (image_data == NULL)
            return false;
        std::cout<<"imageDataFilled"<<std::endl;
        std::cout<<nCol<<" "<<nRow<<"\n";
        // Create a OpenGL texture identifier
        glGenTextures(1, &scanColoredTex);
        glBindTexture(GL_TEXTURE_2D, scanColoredTex);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
            std::cout<<"imageCreated"<<std::endl;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nCol, nRow, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        delete[] image_data;
    std::cout<<"imageCreated"<<std::endl;
        return true;
    }
    void showImage()
    {
        if (!scanColoredTex)
        {
            bool ret = createScanImage();
            IM_ASSERT(ret);
        }
        ImGui::Begin("OpenGL Texture Text");
        ImGui::Text("pointer = %p", this->scanColoredTex);
        ImGui::Text("size = %d x %d", nCol, nRow);
        ImGui::Image((void *)(intptr_t)scanColoredTex, ImVec2(nCol, nRow));
        ImGui::End();
    }
};

struct ImageData
{
    int index;
    e57::Image2D header;
    e57::Image2DProjection imageProjection; //like E57_SPHERICAL
    e57::Image2DType imageType;             //like E57_JPEG_IMAGE
    int64_t nImageWidth = 0;
    int64_t nImageHeight = 0;
    int64_t nImagesSize = 0;          //Number of bytes in the image
    e57::Image2DType imageMaskType;   //like E57_PNG_IMAGE_MASK if present
    e57::Image2DType imageVisualType; //like E57_JPEG_IMAGE if present
    std::vector<uint8_t> data;
};

class e57Scan
{
    std::string filePath;
    e57::E57Root fileTopHeader;
    std::vector<ScanData> scans;
    std::vector<ImageData> images;

public:
    e57Scan();
    const std::string &getFilePath() const;
    int load(const std::string &filename);
    const e57::E57Root &getTopHeader() const { return fileTopHeader; };
    const std::vector<ScanData> &getScans() const { return scans; };
    std::vector<ScanData> &getScans() { return scans; };
};
