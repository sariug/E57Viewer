#include "e57Scan.h"

class ScanManager
{
    std::vector<std::shared_ptr<e57Scan>> m_scans;  
public:
    ScanManager();
    int addScan(const std::string& filename);
    void showScanInfo() const
    {
        
    }
};