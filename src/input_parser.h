#include <algorithm>
#include <string>
#include <vector>

class InputParser {
    public:
        InputParser(int &argc, char **argv) {
            for (int i = 1; i < argc; i++)
                this->tokens_.push_back(std::string(argv[i]));
        }

        const std::string& cmd_option(const std::string &option) const {
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(this->tokens_.begin(), this->tokens_.end(), option);
            if (itr != this->tokens_.end() && ++itr != this->tokens_.end()){
                return *itr;
            }

            static const std::string kEmptyString("");
            return kEmptyString;
        }

        bool OptionExists(const std::string &option) const {
            return std::find(this->tokens_.begin(), this->tokens_.end(), option)
                   != this->tokens_.end();
        }

    private:
        std::vector<std::string> tokens_;
};
