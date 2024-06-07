//
// Created by demian on 6/6/24.
//

#include "i_beacon.h"

iBeacon::iBeacon(Configuration &configuration) {
    bluetooth_socket_ = hci_open_dev(hci_get_route(NULL));
    if (bluetooth_socket_ < 0) {
        throw std::runtime_error("Cannot open BLE devices.");
    }

    SetConfiguration(configuration);
    SetAdvertisingParameters();
    SetAdvertisingData();
}

void iBeacon::SetConfiguration(Configuration &configuration) {
    tx_power_ = configuration.tx_power();
    advertising_interval_ = configuration.advertising_interval();
}

void iBeacon::SetAdvertisingParameters() {
    le_set_advertising_parameters_cp adv_params_cp;
    std::memset(&adv_params_cp, 0, sizeof(adv_params_cp));
    adv_params_cp.min_interval = htobs(0x0800);
    adv_params_cp.max_interval = htobs(0x0800);
    adv_params_cp.advtype = 0x00;
    adv_params_cp.own_bdaddr_type = 0x00;
    adv_params_cp.chan_map = 0x07;
    adv_params_cp.filter = 0x00;

    if (hci_send_cmd(bluetooth_socket_, OGF_LE_CTL, OCF_LE_SET_ADVERTISING_PARAMETERS, LE_SET_ADVERTISING_PARAMETERS_CP_SIZE, &adv_params_cp) < 0) {
        throw std::runtime_error("Failed to set advertising parameters.");    
    }
}

void iBeacon::SetAdvertisingData() {
    le_set_advertising_data_cp adv_data_cp;
    std::memset(&adv_data_cp, 0, sizeof(adv_data_cp));
    adv_data_cp.data[0] = 0x02; // Length
    adv_data_cp.data[1] = 0x01; // Flags
    adv_data_cp.data[2] = 0x1A; // General discoverable mode and BR/EDR not supported

    adv_data_cp.data[3] = 0x1A; // Length
    adv_data_cp.data[4] = 0xFF; // Manufacturer specific data
    adv_data_cp.data[5] = 0x4C; // Apple, Inc.
    adv_data_cp.data[6] = 0x00; // Apple, Inc.
    adv_data_cp.data[7] = 0x02; // iBeacon
    adv_data_cp.data[8] = 0x15; // Length of remaining data
    
    // std::memcpy(&adv_data_cp.data[9], uuid_, sizeof(uuid_)); // UUID

    // Major
    adv_data_cp.data[25] = 0x00;
    adv_data_cp.data[26] = 0x01;

    // Minor
    adv_data_cp.data[27] = 0x00;
    adv_data_cp.data[28] = 0x01;

    // TX power
    adv_data_cp.data[29] = tx_power_;

    adv_data_cp.length = 30;

    if (hci_send_cmd(bluetooth_socket_, OGF_LE_CTL, OCF_LE_SET_ADVERTISING_DATA, LE_SET_ADVERTISING_DATA_CP_SIZE, &adv_data_cp) < 0) {
        throw std::runtime_error("Failed to set advertising data.");
    }
}
