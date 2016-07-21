#include <stdio.h>
#include <iostream>
#include <fstream>
#include "tapplication.h"

int main(int argc, char *argv[])
{
    TApplication app;

    app.init(argc, argv);
    return app.run();
}
