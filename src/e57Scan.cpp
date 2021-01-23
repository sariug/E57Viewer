#include "../include/e57Scan.h"
#include "E57SimpleReader.h"

e57Scan::e57Scan()
{
    std::cout<<"Initiated the scan-\n";
}

void e57Scan::load()
{
    try
    {
	e57::Reader 	eReader("file.e57");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}