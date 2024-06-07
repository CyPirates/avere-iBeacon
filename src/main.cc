#include <iostream>
#include "configuration.h"
#include "i_beacon.h"

int main(int argc, char **argv) {
    auto& configuration = Configuration::GetInstance();
    configuration.Parse(argc, argv);

    auto i_beacon = iBeacon(configuration);

    return 0;
}
