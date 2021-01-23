#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "E57SimpleReader.h"
class e57Scan
{
    e57::E57Root fileTopHeader;
    std::vector<e57::Data3D> dataBlockHeaders; 
public:
    e57Scan();

    void load(const std::string &filename);
};