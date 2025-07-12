#include "JsonParser.h"
#include "JsonParserConfig.h"
int main() {
    const char* dir_input = "./src/input_json";
    const char* dir_output = "./src/output_json";
    std::string file_name;
    std::cout << "Version: " << Tutorial_VERSION_MAJOR << "." << Tutorial_VERSION_MINOR << "." << Tutorial_VERSION_PATCH
              << std::endl;
    while (true) {
        std::cout << "parse json file, or input 1 to exit: " << std::endl;
        std::cin >> file_name;
        if (file_name == "1") {
            break;
        }
        std::string file_input = std::string(dir_input) + '/' + file_name;
        std::string file_output = std::string(dir_output) + '/' + file_name;
        JsonParser::KJson Parser(file_input);
        try {
            JsonObject obj(Parser.parse());
            std::cout << obj.toString() << std::endl;
            obj.out_to_file(file_output);
            obj.clear();
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    return 0;
}