#include <iostream>
#include "igsmr_config.h"

using namespace std;

int main(int argc, char *argv[])
{
    IgsmrConfig::initialize(argc, argv); 

    IgsmrConfig::getInstance().print(cout);

    return 0;
}

