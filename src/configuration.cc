#include "configuration.h"

Configuration& Configuration::GetInstance() {
    static Configuration instance;
    return instance;
}

void Configuration::Parse(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        this->tokens_.push_back(std::string(argv[i]));
    }
    
    try {
        uuid_ = FindArgument("-u");
    } catch (...) {
        uuid_ = std::string("");
    }

    try {
        tx_power_ = std::stoi(FindArgument("-p"));
    } catch (...) {
        tx_power_ = 0xc5;
    }

    try {
        advertising_interval_ = std::stoi(FindArgument("-i"));
    } catch (...) {
        advertising_interval_ = 5 * 60 * 100;
    }
}

std::string Configuration::uuid() {
    return uuid_;
}

int Configuration::tx_power() {
    return tx_power_;
}

int Configuration::advertising_interval() {
    return advertising_interval_;
}

const std::string& Configuration::FindArgument(std::string option) {
    std::vector<std::string>::const_iterator argument_iterator;
    argument_iterator = std::find(this->tokens_.begin(), this->tokens_.end(), option);

    if (argument_iterator != this->tokens_.end() && ++argument_iterator != this->tokens_.end())
        return *argument_iterator;
    
    throw CommandLineParsingException(std::string("No argument for ") + option);
}
