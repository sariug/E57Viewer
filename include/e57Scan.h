#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include "E57SimpleReader.h"
#include <GL/glew.h>
#include "imgui.h"

class ImageData;
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
        return pointCloud.at(col * nRow + row);
    }
    bool showPointCloud = false;
    int selectionToShowScanPoint = 0, lastSelectionToShowScanPoints = 0;

    e57::Data3D header;
    std::vector<ImageData> images;
    GLuint scanTex;

    enum ImageType
    {
        None,
        Color,
        Intensity,
        InvalidPoints
    };

    // Simple helper function to load an image into a OpenGL texture with common settings
    bool createScanImage(ImageType type)
    {
        if (type == None)
            return false;
        uint8_t *image_data = new uint8_t[nCol * nRow * 4]; //stbi_load(filename, &image_width, &image_height, NULL, 4);
        int c = nCol * nRow * 4;
        for (int i = 0; i < nRow; i++)
            for (int j = 0; j < nCol; j++)
            {
                image_data[--c] = 255; // Alpha
                // Rotate J. The whole reason of this drama is that
                // (i,j)=(0,0) of a scan is top left. Texture 0,0 is bottom left. We first rotated all around. But we just needed upside down.
                const auto pt = getPt(nCol - j - 1, i);
                float isEmptyPt = (pt.x == 0 && pt.y == 0 && pt.z == 0) ? 0 : pt.i;
                if (isEmptyPt > 0)
                {
                    switch (type)
                    {
                    case ImageType::Intensity:
                        for (auto _t = 0; _t < 3; _t++)
                            image_data[--c] = pt.i * 255;
                        break;
                    case ImageType::InvalidPoints:
                        // Black
                        for (auto _t = 0; _t < 3; _t++)
                            image_data[--c] = 0;
                        break;
                    case ImageType::Color:
                        image_data[--c] = pt.b;
                        image_data[--c] = pt.g;
                        image_data[--c] = pt.r;
                        break;
                    }
                }
                else
                {
                    switch (type)
                    {
                    case ImageType::InvalidPoints:
                        image_data[--c] = 255;
                        image_data[--c] = 180;
                        image_data[--c] = 0;
                        break;
                    default:
                        for (auto _t = 0; _t < 3; _t++)
                            image_data[--c] = 0;
                    }
                }
            }
        if (image_data == NULL)
            return false;
        std::cout << "imageDataFilled" << std::endl;
        // Create a OpenGL texture identifier
        glGenTextures(1, &scanTex);
        glBindTexture(GL_TEXTURE_2D, scanTex);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
        std::cout << "imageCreated" << std::endl;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nCol, nRow, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        delete[] image_data;
        std::cout << "imageCreated" << std::endl;
        return true;
    }
    void renderScans()
    {
        ImGui::Combo("Property", &selectionToShowScanPoint, "None\0Color\0Intensity\0InvalidPoints\0");

        if (lastSelectionToShowScanPoints != selectionToShowScanPoint)
            scanTex = 0;
        if (!scanTex)
        {
            lastSelectionToShowScanPoints = selectionToShowScanPoint;
            bool ret = createScanImage(static_cast<ImageType>(selectionToShowScanPoint));
            if (!ret)
                return;
        }
        ImGui::Begin("ScanView");
        ImGui::Text("size = %d x %d", nCol, nRow);
        ImGui::Image((void *)(intptr_t)scanTex, ImVec2(nCol, nRow));
        ImGui::End();
    }
};

struct ImageData
{
    bool render = false;
    int64_t index;
    e57::Image2D header;
    e57::Image2DProjection imageProjection; //like E57_SPHERICAL
    e57::Image2DType imageType;             //like E57_JPEG_IMAGE
    int64_t nImageWidth = 0;
    int64_t nImageHeight = 0;
    int64_t nImagesSize = 0;          //Number of bytes in the image
    e57::Image2DType imageMaskType;   //like E57_PNG_IMAGE_MASK if present
    e57::Image2DType imageVisualType; //like E57_JPEG_IMAGE if present
    unsigned char *data;
    GLuint imageTex = 0;
    void display()
    {
        std::cout<<header.name<<std::endl;
        if (!imageTex)
        {
            std::cout << "imageGenerate" << std::endl;
            // Create a OpenGL texture identifier
            glGenTextures(1, &imageTex);
            glBindTexture(GL_TEXTURE_2D, imageTex);

            // Setup filtering parameters for display
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nImageWidth, nImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
        }
        ImGui::Begin("Image");
        ImGui::Text("size = %d x %d", nImageWidth, nImageHeight);
        ImGui::Image((void *)(intptr_t)imageTex, ImVec2(nImageWidth, nImageHeight));
        ImGui::End();
    }
};

class e57Scan
{
    std::string filePath;
    e57::E57Root fileTopHeader;
    std::vector<ScanData> scans;
    std::vector<std::shared_ptr<ImageData>> images;

public:
    e57Scan();
    const std::string &getFilePath() const;
    int load(const std::string &filename);
    const e57::E57Root &getTopHeader() const { return fileTopHeader; };
    const std::vector<ScanData> &getScans() const { return scans; };
    std::vector<ScanData> &getScans() { return scans; };
};
