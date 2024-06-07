//
// Created by demian on 6/6/24.
//

#include "i_beacon.h"

iBeacon::iBeacon(Configuration &configuration) {
    bluetooth_socket_ = hci_open_dev(hci_get_route(NULL));
    if (bluetooth_socket_ < 0) {
        throw iBeaconError("Cannot open BLE devices");
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
        throw iBeaconError("Failed to set advertising parameters");
    }
}

void iBeacon::SetAdvertisingData() {
    std::memset(&advertising_data_, 0, sizeof(advertising_data_));
    advertising_data_.data[0] = 0x02; // Length
    advertising_data_.data[1] = 0x01; // Flags
    advertising_data_.data[2] = 0x1A; // General discoverable mode and BR/EDR not supported

    advertising_data_.data[3] = 0x1A; // Length
    advertising_data_.data[4] = 0xFF; // Manufacturer specific data
    advertising_data_.data[5] = 0x4C; // Apple, Inc.
    advertising_data_.data[6] = 0x00; // Apple, Inc.
    advertising_data_.data[7] = 0x02; // iBeacon
    advertising_data_.data[8] = 0x15; // Length of remaining data
    
    // std::memcpy(&advertising_data_[9], uuid_, sizeof(uuid_)); // UUID

    // Major
    advertising_data_.data[25] = 0x00;
    advertising_data_.data[26] = 0x01;

    // Minor
    advertising_data_.data[27] = 0x00;
    advertising_data_.data[28] = 0x01;

    // TX power
    advertising_data_.data[29] = tx_power_;

    advertising_data_.length = 30;

    if (hci_send_cmd(bluetooth_socket_, OGF_LE_CTL, OCF_LE_SET_ADVERTISING_DATA, LE_SET_ADVERTISING_DATA_CP_SIZE, &advertising_data_) < 0) {
        throw iBeaconError("Failed to set advertising data");
    }
}

void iBeacon::StartAdvertising() {
    StopAdvertising();
    
    if (hci_le_set_advertise_enable(bluetooth_socket_, 0x01, advertising_interval_) < 0) {
        throw iBeaconError("Failed to start iBeacon");
    }

    int advertising_interval_seconds = advertising_interval_ / 100;
    std::cout << "[+] iBeacon started... Running for " << advertising_interval_seconds << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(advertising_interval_seconds));

    if (StopAdvertising() != true) {
        throw iBeaconError("Failed to stop iBeacon");
    }
    close(bluetooth_socket_);
    std::cout << "[+] iBeacon stopped... Socket closed\n";
}

bool iBeacon::StopAdvertising() {
    return hci_le_set_advertise_enable(bluetooth_socket_, 0x00, 1000) == 0;
}

