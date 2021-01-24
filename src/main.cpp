#include "../include/e57Viewer.h"

int main(int, char **)
{

    auto &viewer = e57Viewer::instance();
    viewer.run();
    viewer.prepareShutdown();

    return 0;
}