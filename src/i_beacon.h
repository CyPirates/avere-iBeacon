//
// Created by demian on 6/6/24.
//

#ifndef AVERE_IBEACON_I_BEACON_H
#define AVERE_IBEACON_I_BEACON_H

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "configuration.h"

class iBeacon {
public:
    iBeacon(Configuration& configuration);

    void StartAdvertising();

private:
    void SetConfiguration(Configuration& configuration);
    void SetAdvertisingParameters();
    void SetAdvertisingData();

    void StopAdvertising();

    int bluetooth_socket_{-1};
    uint uuid_[0x10] = { 0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, 0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0 };
    int tx_power_{0xc5};
    int advertising_interval_{5 * 60 * 100};
};


#endif //AVERE_IBEACON_I_BEACON_H
