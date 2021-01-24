#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "E57SimpleReader.h"

struct scanData{
    std::int64_t nCol, nRow, nTotalPoints, nGroupSize, nPointsPerGroup;
    bool colIndex = true;
    e57::Data3DPointsData data;
    e57::Data3D header;

};

class e57Scan
{
    e57::E57Root fileTopHeader;
    std::vector<scanData> scans; 
public:
    e57Scan();

    int load(const std::string &filename);
};