#include "../include/e57Scan.h"

e57Scan::e57Scan()
{
    std::cout << "Initiated the scan-\n";
}

void e57Scan::load(const std::string &filename)
{
    try
    {
        std::unique_ptr<e57::Reader> eReader = std::make_unique<e57::Reader>("pumpACartesian.e57");
        eReader->GetE57Root(fileTopHeader);

        std::cout << filename << " is being loaded.\n";
        std::cout << fileTopHeader.coordinateMetadata << std::endl;

        dataBlockHeaders.resize(eReader->GetData3DCount());
        for(auto c = 0; c<dataBlockHeaders.size();c++)
        {
            eReader->ReadData3D(c, dataBlockHeaders[c]);
            std::cout<<dataBlockHeaders[c].name<<std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}