#ifndef _JSONPARSER_H_
#define _JSONPARSER_H_

#include "JObject.h"
#include <sstream>
#include <stdexcept>
using namespace JObject;

namespace JsonParser {
    class KJson {
      private:
        int v_index;
        std::string v_json_str;
        JsonObject parse_bool();
        JsonObject parse_null();
        JsonObject parse_number();
        JsonObject parse_string();
        JsonObject parse_array();
        JsonObject parse_object();

      public:
        KJson() : v_index(0), v_json_str("") {}
        KJson(const std::string& filename) : v_index(0) { load_file(filename); }
        void load_file(const std::string& filename);
        JsonObject parse();
        bool is_whitespace(const char& c) const;
        void skip_whitespace();
        void print() { std::cout << v_json_str << std::endl; }
    };
}  // namespace JsonParser

#endif