#include "../include/e57Viewer.h"


int main(int, char**)
{

    std::unique_ptr<e57Viewer> viewer(new e57Viewer);
    viewer->run();
    return 0;
}