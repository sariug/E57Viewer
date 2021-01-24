#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "E57SimpleReader.h"

struct ScanPt
{
    float x=0.0,y=0.0,z=0.0,i=0.0;
    uint8_t r=0,g=0,b=0;
};
struct ScanData
{
    std::int64_t nCol, nRow, nTotalPoints, nGroupSize, nPointsPerGroup;
    bool colIndex = true;
    std::vector<ScanPt> pointCloud;
    void setPt(int col, int row, ScanPt& pt){
        pointCloud.at(col*nRow+row) = pt;
    }
    ScanPt& getPt(int col, int row){
        return pointCloud.at(col*nRow+row);
    }
    e57::Data3D header;

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
