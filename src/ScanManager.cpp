#include "../include/ScanManager.h"

ScanManager::ScanManager()
{

}

int ScanManager::addScan(const std::string& filename)
{
    m_scans.push_back(std::make_unique<e57Scan>());
    m_scans.back()->load(filename);
}
