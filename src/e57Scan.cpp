#include "../include/e57Scan.h"

e57Scan::e57Scan()
{
    std::cout << "Initiated the scan-\n";
}

int e57Scan::load(const std::string &filename)
{
    try
    {
        std::unique_ptr<e57::Reader> eReader = std::make_unique<e57::Reader>("pumpACartesian.e57");
        
        if(!eReader->IsOpen())
            return 1;


        
        eReader->GetE57Root(fileTopHeader);

        std::cout << filename << " is being loaded.\n";
        std::cout << fileTopHeader.coordinateMetadata << std::endl;

        scans.resize(eReader->GetData3DCount());
        for (auto scanIdx= 0; scanIdx < scans.size(); scanIdx++)
        {
            eReader->ReadData3D(scanIdx, scans[scanIdx].header);
            std::cout << scans[scanIdx].header.name << std::endl;

            scanData s;
            eReader->GetData3DSizes(scanIdx, s.nRow, s.nCol, s.nTotalPoints,
                                   s.nGroupSize, s.nPointsPerGroup, s.colIndex);

            // bufer size check and update(http://www.libe57.org/TutorialSimpleAPI.html)
            int64_t bufferSize = (s.nRow > 0) ? s.nRow : 1024;	




        }


            // Reader::GetData3DCount() const

     
            // bool
            // Reader::GetData3DSizes(int64_t dataIndex, int64_t & rowMax, int64_t & columnMax, int64_t & pointsSize,
            //                        int64_t & groupsSize, int64_t & countSize, bool &bColumnIndex) const

            // bool
            // Reader::ReadData3DGroupsData(int64_t dataIndex, int64_t groupCount, int64_t * idElementValue,
            //                              int64_t * startPointIndex, int64_t * pointCount) const

            // CompressedVectorReader
            // Reader::SetUpData3DPointsData(int64_t dataIndex, size_t pointCount,
            //                               const Data3DPointsData &buffers) const

            // CompressedVectorReader
            // Reader::SetUpData3DPointsData(int64_t dataIndex, size_t pointCount,
            //                               const Data3DPointsData_d &buffers) const
    
        
            // int64_t
            // Reader::GetImage2DCount() const bool
            // Reader::ReadImage2D(int64_t imageIndex, Image2D & image2DHeader) const bool
            // Reader::GetImage2DSizes(int64_t imageIndex, Image2DProjection & imageProjection, Image2DType & imageType,
            //                         int64_t & imageWidth, int64_t & imageHeight, int64_t & imageSize,
            //                         Image2DType & imageMaskType, Image2DType & imageVisualType) const int64_t
            // Reader::ReadImage2DData(int64_t imageIndex, Image2DProjection imageProjection, Image2DType imageType,
            //                         void *pBuffer, int64_t start, int64_t count) const int64_t
        
        
        eReader->Close();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}