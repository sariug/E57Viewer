#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "E57SimpleReader.h"

struct ScanData
{
    std::int64_t nCol, nRow, nTotalPoints, nGroupSize, nPointsPerGroup;
    bool colIndex = true;
    std::vector<float> x, y, z, i;
    std::vector<uint8_t> r, g, b;
    std::vector<int8_t> invCart, invSphere;
    std::vector<int32_t> row, col;

    e57::Data3D header;

    double getIntensity(int index)
    {
        return (i[index] - header.intensityLimits.intensityMinimum) / (header.intensityLimits.intensityMaximum - header.intensityLimits.intensityMinimum);
    }
    int getRed(int index)
    {
        return ((r[index] - header.colorLimits.colorRedMinimum) * 255) / (header.colorLimits.colorRedMaximum - header.colorLimits.colorRedMinimum);
    }
    int getGreen(int index)
    {
        return ((g[index] - header.colorLimits.colorGreenMinimum) * 255) / (header.colorLimits.colorGreenMaximum - header.colorLimits.colorGreenMinimum);
    }
    int getBlue(int index)
    {
        return ((b[index] - header.colorLimits.colorBlueMinimum) * 255) / (header.colorLimits.colorBlueMaximum - header.colorLimits.colorBlueMinimum);
    }
};

class e57Scan
{
    e57::E57Root fileTopHeader;
    std::vector<ScanData> scans;

public:
    e57Scan();

    int load(const std::string &filename);
    const e57::E57Root &getTopHeader() const { return fileTopHeader; };
    const std::vector<ScanData> &getScans() const { return scans; };
};
