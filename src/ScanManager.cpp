#include "../include/ScanManager.h"

ScanManager::ScanManager()
{

}

int ScanManager::addScan(const std::string& filename)
{
    m_scans.push_back(std::make_unique<e57Scan>());
    m_scans.back()->load(filename);
}

void ScanManager::showScanInfo() const
  {
        auto writeToTable = [](const std::string &colName, const std::string &colValue, bool backgroudRed = false) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(colName.c_str());
            if (backgroudRed)
            {
                ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.7f, 0.3f, 0.3f, 0.65f));
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
            }
            ImGui::TableNextColumn();
            ImGui::Text("%s", colValue.c_str());
            if (backgroudRed)
            {
                ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.7f, 0.3f, 0.3f, 0.65f));
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
            }
        };
        for (auto &s : m_scans)
        {
            if (ImGui::TreeNode("Top Level Header"))
            {
                if (ImGui::BeginTable("##table2", 2, ImGuiTableColumnFlags_WidthStretch))
                {

                    auto &th = s->getTopHeader();
                    writeToTable("Format Name", th.formatName);
                    writeToTable("GUID", th.guid);
                    writeToTable("Version Major", std::to_string(th.versionMajor));
                    writeToTable("Version Minor", std::to_string(th.versionMinor));
                    writeToTable("creationDateTime(GPS)", std::to_string(th.creationDateTime.dateTimeValue));
                    writeToTable("data3DSize", std::to_string(th.data3DSize));
                    writeToTable("images2DSize", std::to_string(th.images2DSize));
                    writeToTable("coordinateMetadata", th.coordinateMetadata);
                }
                ImGui::EndTable();
                ImGui::TreePop();
            }
            auto &scanContainer = s->getScans();
            for (auto &sh : scanContainer)
            {

                auto &header = sh.header;
                if (ImGui::TreeNode(header.name.c_str()))
                {
                    if (ImGui::BeginTable("##table3", 2, ImGuiTableColumnFlags_WidthStretch))
                    {
                        writeToTable("Name", header.name);
                        writeToTable("GUID", header.guid);
                        writeToTable("originalGuids", std::accumulate(header.originalGuids.begin(), header.originalGuids.end(), std::string{},
                                                                      [](std::string &s, const std::string &piece) -> decltype(auto) { return s += piece + " "; }));
                        writeToTable("Sensor Vendor", header.sensorVendor);
                        writeToTable("Sensor Model", header.sensorModel);
                        writeToTable("Sensor Serial Number", header.sensorSerialNumber);
                        writeToTable("Sensor Hardware Version", header.sensorHardwareVersion);
                        writeToTable("Sensor Software Version", header.sensorSoftwareVersion);
                        writeToTable("Sensor Firmware Version", header.sensorFirmwareVersion);
                        writeToTable("Temperature", std::to_string(header.temperature));
                        writeToTable("Relative \n Humidity", std::to_string(header.relativeHumidity));
                        writeToTable("Atmospheric Pressure", std::to_string(header.atmosphericPressure));
                        writeToTable("Acquisition Start", std::to_string(header.acquisitionStart.dateTimeValue));
                        writeToTable("Acquisition End", std::to_string(header.acquisitionEnd.dateTimeValue));
                        writeToTable("Pose", "Value", true);
                        writeToTable("\tRotation", std::to_string(header.pose.rotation.w) + " " + std::to_string(header.pose.rotation.x) + " " + std::to_string(header.pose.rotation.y) + " " + std::to_string(header.pose.rotation.z));
                        writeToTable("\tTranslation: ", std::to_string(header.pose.translation.x) + " " + std::to_string(header.pose.translation.y) + " " + std::to_string(header.pose.translation.z));

                        writeToTable("Index Bounds", "Value", true);
                        writeToTable("\tRow Max", std::to_string(header.indexBounds.rowMaximum));
                        writeToTable("\tRow Min", std::to_string(header.indexBounds.rowMinimum));
                        writeToTable("\tColumn Max", std::to_string(header.indexBounds.columnMaximum));
                        writeToTable("\tColumn Min", std::to_string(header.indexBounds.columnMinimum));
                        writeToTable("\tReturn Max", std::to_string(header.indexBounds.returnMaximum));
                        writeToTable("\tReturn Min", std::to_string(header.indexBounds.returnMinimum));

                        writeToTable("Cartesian Bounds", "Value", true);
                        writeToTable("\tX", ("[" + std::to_string(header.cartesianBounds.xMinimum) + ", " + std::to_string(header.cartesianBounds.xMaximum) + "]"));
                        writeToTable("\tY", ("[" + std::to_string(header.cartesianBounds.yMinimum) + ", " + std::to_string(header.cartesianBounds.yMaximum) + "]"));
                        writeToTable("\tZ", ("[" + std::to_string(header.cartesianBounds.zMinimum) + ", " + std::to_string(header.cartesianBounds.zMaximum) + "]"));

                        writeToTable("Spherical Bounds", "Value", true);
                        writeToTable("\tAzimuth", ("[" + std::to_string(header.sphericalBounds.azimuthStart) + ", " + std::to_string(header.sphericalBounds.azimuthEnd) + "]"));
                        writeToTable("\tElevation", ("[" + std::to_string(header.sphericalBounds.elevationMinimum) + ", " + std::to_string(header.sphericalBounds.elevationMaximum) + "]"));
                        writeToTable("\tRange", ("[" + std::to_string(header.sphericalBounds.rangeMinimum) + ", " + std::to_string(header.sphericalBounds.rangeMaximum) + "]"));

                        writeToTable("Intensity & Color Limits", "Value", true);
                        writeToTable("\tRed", ("[" + std::to_string(header.colorLimits.colorRedMinimum) + ", " + std::to_string(header.colorLimits.colorRedMaximum) + "]"));
                        writeToTable("\tGreen", ("[" + std::to_string(header.colorLimits.colorGreenMinimum) + ", " + std::to_string(header.colorLimits.colorGreenMaximum) + "]"));
                        writeToTable("\tBlue", ("[" + std::to_string(header.colorLimits.colorBlueMinimum) + ", " + std::to_string(header.colorLimits.colorBlueMaximum) + "]"));
                        writeToTable("\tIntensity", ("[" + std::to_string(header.intensityLimits.intensityMinimum) + ", " + std::to_string(header.intensityLimits.intensityMaximum) + "]"));

                        writeToTable("Point Grouping Schemas", "NOT IMPLEMENTED");

                        writeToTable("Number of Points", std::to_string(header.pointsSize));
                    }

                    ImGui::EndTable();
                    if (ImGui::TreeNode("Available Fields"))
                    {
                        if (ImGui::BeginTable("##table4", 2, ImGuiTableColumnFlags_WidthStretch))
                        {
                            const auto &fields = header.pointFields;
                            const std::string yes = "yes", no = "no";
                            writeToTable("\tCartesian X", fields.cartesianXField ? yes : no);
                            writeToTable("\tCartesian Y", fields.cartesianYField ? yes : no);
                            writeToTable("\tCartesian Z", fields.cartesianZField ? yes : no);
                            writeToTable("\tCartesian Invalid State", fields.cartesianInvalidStateField ? yes : no);
                            writeToTable("\tSpherical Azimuth", fields.sphericalAzimuthField ? yes : no);
                            writeToTable("\tSpherical Elevation", fields.sphericalElevationField ? yes : no);
                            writeToTable("\tSpherical Range", fields.sphericalRangeField ? yes : no);
                            writeToTable("\tSpherical Invalid State", fields.sphericalInvalidStateField ? yes : no);
                            writeToTable("\tPoint Range", ("[" + std::to_string(fields.pointRangeMinimum) + ", " + std::to_string(fields.pointRangeMaximum) + "]"));
                            writeToTable("\tPoint Range Scaled Integer", std::to_string(fields.pointRangeScaledInteger));
                            writeToTable("\tAngle Range", ("[" + std::to_string(fields.angleMinimum) + ", " + std::to_string(fields.angleMaximum) + "]"));
                            writeToTable("\tAngle Range Scaled Integer", std::to_string(fields.angleScaledInteger));
                            writeToTable("\tColumn Index Field", fields.columnIndexField ? yes : no);
                            writeToTable("\tColumn Index Maximum", std::to_string(fields.columnIndexMaximum));

                            writeToTable("\tRow Index Field", fields.rowIndexField ? yes : no);
                            writeToTable("\tRow Index Maximum", std::to_string(fields.rowIndexMaximum));

                            writeToTable("\tReturn Index Field", fields.returnIndexField ? yes : no);
                            writeToTable("\tReturn Count Field", fields.returnCountField ? yes : no);
                            writeToTable("\tReturn Maximum", std::to_string(fields.returnMaximum));

                            writeToTable("\tIntensity Field", fields.intensityField ? yes : no);
                            writeToTable("\tIs Intensity Invalid", fields.isIntensityInvalidField ? yes : no);
                            writeToTable("\tIntensity Scaled Integer", std::to_string(fields.intensityScaledInteger));

                            writeToTable("\tColor Red", fields.colorRedField ? yes : no);
                            writeToTable("\tColor Green", fields.colorGreenField ? yes : no);
                            writeToTable("\tColor Blue", fields.colorBlueField ? yes : no);
                            writeToTable("\tIs Color Invalid", fields.isColorInvalidField ? yes : no);

                            writeToTable("\tNormal X available", fields.normalX ? yes : no);
                            writeToTable("\tNormal Y available", fields.normalY ? yes : no);
                            writeToTable("\tNormal Z available", fields.normalZ ? yes : no);
                            writeToTable("TIME STAMP FIELDS", "NOT IMPLEMENTED");

                            ImGui::EndTable();
                        }

                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
            }
        }
    }
