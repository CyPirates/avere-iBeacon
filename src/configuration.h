#ifndef CONFIGURATION
#define CONFIGURATION
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <stdexcept>

class Configuration {
public:
    /**
     * Singleton
     */
    static Configuration& GetInstance();

    void Parse(int argc, char** argv);

    /**
     * Getter
     */
    std::string uuid();
    int tx_power();
    int advertising_interval();

private:
    Configuration();
    Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
	Configuration(Configuration&&) = delete;
	Configuration& operator=(Configuration&&) = delete;

    const std::string& FindArgument(std::string option);

    std::vector<std::string> tokens_;
    std::string uuid_;
    int tx_power_;
    int advertising_interval_;

    /**
     * Command line argument exception when parsing
     */
    class CommandLineParsingException: public std::runtime_error {
        public:
            CommandLineParsingException(const std::string message) : std::runtime_error(message) {};     
    };
};

#endif
