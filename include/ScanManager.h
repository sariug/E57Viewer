#include "e57Scan.h"
#include "imgui.h"
#include <numeric>
class ScanManager
{
    std::vector<std::shared_ptr<e57Scan>> m_scans;

public:
    ScanManager();
    int addScan(const std::string &filename);
    void showScanInfo() const;
};