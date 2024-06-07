//
// Created by demian on 6/6/24.
//

#include "i_beacon.h"

iBeacon::iBeacon(Configuration &configuration) {
    bluetooth_socket_ = hci_open_dev(hci_get_route(NULL));
    SetConfiguration(configuration);
}

void iBeacon::SetConfiguration(Configuration &configuration) {
    tx_power_ = configuration.tx_power();
    advertising_interval_ = configuration.advertising_interval();
}