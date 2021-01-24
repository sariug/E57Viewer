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

        const auto &availableFields = scan.header.pointFields;
        const bool cartesianAvailable = availableFields.cartesianXField && availableFields.cartesianYField && availableFields.cartesianZField;
        const bool colorAvailable = availableFields.colorRedField && availableFields.colorGreenField && availableFields.colorBlueField;
        const bool intensityAvailable = availableFields.intensityField;
        // Construction variables.
        std::vector<float> x, y, z, intensity;
        std::vector<uint8_t> r, g, b;
        std::vector<int8_t> invCart, invSphere;
        std::vector<int32_t> row, col;
        if (cartesianAvailable)
        {
            x.reserve(bufferSize);
            y.reserve(bufferSize);
            z.reserve(bufferSize);
            if (availableFields.cartesianInvalidStateField)
                invCart.reserve(bufferSize);
        }
        if (colorAvailable)
        {
            r.reserve(bufferSize);
            g.reserve(bufferSize);
            b.reserve(bufferSize);
        }
        if (intensityAvailable)
            intensity.reserve(bufferSize);

        if (availableFields.rowIndexField)
            row.reserve(bufferSize);
        if (availableFields.columnIndexField)
            col.reserve(bufferSize);
        e57::Data3DPointsData buffer;
        buffer.cartesianX = x.data();
        buffer.cartesianY = y.data();
        buffer.cartesianZ = z.data();
        buffer.colorRed = r.data();
        buffer.colorGreen = g.data();
        buffer.colorBlue = b.data();
        buffer.intensity = intensity.data();
        buffer.columnIndex = col.data();
        buffer.rowIndex = row.data();
        buffer.cartesianInvalidState = invCart.data();
        buffer.sphericalInvalidState = invSphere.data();

        // Setup buffers
        e57::CompressedVectorReader dataReader = eReader->SetUpData3DPointsData(scanIdx, bufferSize, buffer);

        int64_t count = 0;
        unsigned size = 0;
        int32_t temp_col = 0, temp_row = 0;
        scan.pointCloud.resize(scan.nTotalPoints);
        while (size = dataReader.read())
        {
            for (long i = 0; i < size; i++)
            {

                if (scan.header.pointFields.columnIndexField)
                    temp_col = col[i];
                else
                    temp_col = 0; //point cloud case

                if (scan.header.pointFields.rowIndexField)
                    temp_row = row[i];
                else
                    temp_row = count; //point cloud case
                ScanPt &pt = scan.getPt(temp_col, temp_row);
                if (cartesianAvailable)
                {
                    pt.x = x[i];
                    pt.y = y[i];
                    pt.z = z[i];
                }

                if (colorAvailable)
                {
                    const auto &limits = scan.header.colorLimits;
                    pt.r = ((r[i] - limits.colorRedMinimum) * 255) / (limits.colorRedMaximum - limits.colorRedMinimum);
                    pt.g = ((g[i] - limits.colorGreenMinimum) * 255) / (limits.colorGreenMaximum - limits.colorGreenMinimum);
                    pt.b = ((b[i] - limits.colorBlueMinimum) * 255) / (limits.colorBlueMaximum - limits.colorBlueMinimum);
                }

                if (intensityAvailable)
                    pt.i = (intensity[i] - scan.header.intensityLimits.intensityMinimum) / (scan.header.intensityLimits.intensityMaximum - scan.header.intensityLimits.intensityMinimum);

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