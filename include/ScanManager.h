#include "e57scan.h"

class ScanManager
{
    std::vector<std::shared_ptr<e57Scan>> m_scans;  
public:
    ScanManager();
};