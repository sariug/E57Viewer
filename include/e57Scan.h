#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "E57SimpleReader.h"

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
    e57::Data3D header;
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
};
