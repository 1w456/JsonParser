#include "JsonParser.h"
using namespace JsonParser;

JsonObject KJson::parse() {
    if (v_json_str.empty()) {
        throw std::runtime_error("Json is empty");
    }
    skip_whitespace();
    switch (v_json_str[v_index]) {
        case 'n':
            return parse_null();
            break;
        case 't':
        case 'f':
            return parse_bool();
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
            return parse_number();
            break;
        case '"':
            return parse_string();
            break;
        case '[':
            return parse_array();
            break;
        case '{':
            return parse_object();
            break;
        default:
            return JsonObject();
            break;
    }
}

JsonObject KJson::parse_null() {
    if (v_json_str[v_index] != 'n' || v_json_str[v_index + 1] != 'u' || v_json_str[v_index + 2] != 'l' ||
        v_json_str[v_index + 3] != 'l') {
        throw std::runtime_error("Json文件已损坏: 非法的null值");
    }
    v_index += 4;
    skip_whitespace();
    return JsonObject();
}
JsonObject KJson::parse_bool() {
    if (v_json_str[v_index] == 't') {
        if (v_json_str[v_index + 1] != 'r' || v_json_str[v_index + 2] != 'u' || v_json_str[v_index + 3] != 'e') {
            throw std::runtime_error("Json文件已损坏: 非法的true值");
        }
        v_index += 4;
        skip_whitespace();
        return JsonObject(true);
    } else {
        if (v_json_str[v_index] != 'f' || v_json_str[v_index + 1] != 'a' || v_json_str[v_index + 2] != 'l' ||
            v_json_str[v_index + 3] != 's' || v_json_str[v_index + 4] != 'e') {
            throw std::runtime_error("Json文件已损坏: 非法的false值");
        }
        v_index += 5;
        skip_whitespace();
        return JsonObject(false);
    }
}
JsonObject KJson::parse_number() {
    std::string num_str;
    bool is_int = true;
    while (v_index < v_json_str.size() && v_json_str[v_index] != ',' && v_json_str[v_index] != '}' &&
           v_json_str[v_index] != ']' && !is_whitespace(v_json_str[v_index])) {
        if (v_json_str[v_index] == '.') {
            is_int = false;
        }
        num_str += v_json_str[v_index];
        v_index++;
    }
    try {
        skip_whitespace();
        if (is_int) {
            return JsonObject(std::stoi(num_str));
        } else {
            return JsonObject(std::stod(num_str));
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Json文件已损坏: 非法的number值");
    }
}
JsonObject KJson::parse_string() {
    std::string str_val;
    v_index++;
    for (; v_json_str[v_index] != '"'; v_index++) {
        if (v_index >= v_json_str.size()) {
            throw std::runtime_error("Json文件已损坏: 非法的string值");
        }
        str_val += v_json_str[v_index];
    }
    v_index++;
    skip_whitespace();
    return JsonObject(str_val);
}

JsonObject KJson::parse_array() {
    std::vector<JsonObject> array_val;
    v_index++;
    while (v_json_str[v_index] != ']') {
        if (v_index > v_json_str.size()) {
            throw std::runtime_error("Json文件已损坏: 非法的array值");
        }
        skip_whitespace();
        array_val.push_back(parse());
        skip_whitespace();
        if (v_json_str[v_index] == ',') {
            v_index++;
            skip_whitespace();
        }
    }
    v_index++;
    skip_whitespace();
    return JsonObject(array_val);
}
JsonObject KJson::parse_object() {
    std::map<std::string, JsonObject> object_val;
    std::string key;
    v_index++;
    while (v_json_str[v_index] != '}') {
        if (v_index > v_json_str.size()) {
            throw std::runtime_error("Json文件已损坏: 非法的object值");
        }
        skip_whitespace();
        if (v_json_str[v_index] == '"') {
            key = parse_string().getString();
        }
        if (v_json_str[v_index] != ':') {
            throw std::runtime_error("Json文件已损坏: 非法的object值");
        }
        v_index++;
        skip_whitespace();
        object_val.emplace(key, parse());
        skip_whitespace();
        if (v_json_str[v_index] == ',') {
            v_index++;
            skip_whitespace();
        }
    }
    v_index++;
    skip_whitespace();
    return JsonObject(object_val);
}

void KJson::load_file(const std::string& filename) {
    std::ifstream ifs;
    std::stringstream ss;
    std::string line;
    try {
        ifs.open(filename);
        if (!ifs.is_open()) {
            throw std::runtime_error("cann't open file: " + filename);
        }
        while (std::getline(ifs, line)) {
            ss << line;
        }
        v_json_str = ss.str();
    } catch (const std::exception& e) {
        v_json_str = "";
        ifs.close();
        std::cerr << e.what() << std::endl;  // 输出异常信息，与std::cout不同，std::cerr输出不会被缓冲
        return;
    }
    ifs.close();
}

bool KJson::is_whitespace(const char& c) const {
    if (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\v' && c != '\f') {
        return false;
    }
    return true;
}
void KJson::skip_whitespace() {
    while (is_whitespace(v_json_str[v_index])) {
        v_index++;
    }
}