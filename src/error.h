#ifndef AVERE_IBEACON_ERROR_H
#define AVERE_IBEACON_ERROR_H
#include <stdexcept>
#include <cerrno>
#include <string>
#include <cstring>

class ConfigurationError : public std::runtime_error {
    public:
        explicit ConfigurationError(const std::string& message) : runtime_error(message + std::strerror(errno)) {};
};

class iBeaconError : public std::runtime_error {
    public:
         explicit iBeaconError(const std::string& message) : runtime_error(message + std::strerror(errno)) {};
};

#endif // AVERE_IBEACON_ERROR_H
