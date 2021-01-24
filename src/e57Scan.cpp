#include "../include/e57Scan.h"

e57Scan::e57Scan()
{
    std::cout << "Initiated the scan-\n";
}

int e57Scan::load(const std::string &filename)
{
    // try
    // {
    std::unique_ptr<e57::Reader> eReader = std::make_unique<e57::Reader>("pumpARowColumnIndex.e57");

    if (!eReader->IsOpen())
        return 1;

    eReader->GetE57Root(fileTopHeader);

    std::cout << filename << " is being loaded.\n";
    std::cout << fileTopHeader.coordinateMetadata << std::endl;

    scans.resize(eReader->GetData3DCount());
    for (auto scanIdx = 0; scanIdx < scans.size(); scanIdx++)
    {
        eReader->ReadData3D(scanIdx, scans[scanIdx].header);
        std::cout << scans[scanIdx].header.name << std::endl;
        auto &scan = scans[scanIdx];

        eReader->GetData3DSizes(scanIdx, scan.nRow, scan.nCol, scan.nTotalPoints,
                                scan.nGroupSize, scan.nPointsPerGroup, scan.colIndex);

        // bufer size check and update(http://www.libe57.org/TutorialSimpleAPI.html)
        int64_t bufferSize = (scan.nRow > 0) ? scan.nRow : 1024;
        std::cout << "buffer size" << bufferSize << " " << scan.nRow << std::endl;
        const auto &availableFields = scan.header.pointFields;
        const bool cartesianAvailable = availableFields.cartesianXField && availableFields.cartesianYField && availableFields.cartesianZField;
        const bool colorAvailable = availableFields.colorRedField && availableFields.colorGreenField && availableFields.colorBlueField;
        const bool intensityAvailable = availableFields.intensityField;
        if (cartesianAvailable)
        {
            scan.x.reserve(bufferSize);
            scan.y.reserve(bufferSize);
            scan.z.reserve(bufferSize);
            if (availableFields.cartesianInvalidStateField)
                scan.invCart.reserve(bufferSize);
        }
        if (colorAvailable)
        {
            scan.r.reserve(bufferSize);
            scan.g.reserve(bufferSize);
            scan.b.reserve(bufferSize);
        }
        if (intensityAvailable)
            scan.i.reserve(bufferSize);

        if (availableFields.rowIndexField)
            scan.row.reserve(bufferSize);
        if (availableFields.columnIndexField)
            scan.col.reserve(bufferSize);
        e57::Data3DPointsData buffer;
        buffer.cartesianX = scan.x.data();
        buffer.cartesianY = scan.y.data();
        buffer.cartesianZ = scan.z.data();
        buffer.colorRed = scan.r.data();
        buffer.colorGreen = scan.g.data();
        buffer.colorBlue = scan.b.data();
        buffer.intensity = scan.i.data();
        buffer.columnIndex = scan.col.data();
        buffer.rowIndex = scan.row.data();
        buffer.cartesianInvalidState = scan.invCart.data();
        buffer.sphericalInvalidState = scan.invSphere.data();

        // Setup buffers
        e57::CompressedVectorReader dataReader = eReader->SetUpData3DPointsData(scanIdx, bufferSize, buffer);

        int64_t count = 0;
        unsigned size = 0;
        int32_t col = 0, row = 0;

        while (size = dataReader.read())
        {
            for (long i = 0; i < size; i++)
            {

                if (scan.header.pointFields.columnIndexField)
                    col = scan.col[i];
                else
                    col = 0; //point cloud case

                if (scan.header.pointFields.rowIndexField)
                    row = scan.row[i];
                else
                    row = count; //point cloud case

                    std::cout << scan.row[i]<< " " << col << " " << scan.x[i] << " " << scan.y[i] << " " << scan.z[i] << std::endl;

                count++;
            }
        }
    }

    eReader->Close();
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << e.what() << '\n';
    // }
}