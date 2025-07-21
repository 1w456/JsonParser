#include "JObject.h"
using namespace JObject;

JsonObject::JsonObject(const std::string& v) : type(ValueType::T_string) { value.V_string = new std::string(v); }

JsonObject::JsonObject(const char* v) : type(ValueType::T_string) { value.V_string = new std::string(v); }

JsonObject::JsonObject(const std::vector<JsonObject>& v) : type(ValueType::T_array) {
    value.V_array = new std::vector<JsonObject>;  // 深拷贝，多层嵌套会不会有问题？取决于JsonObject的拷贝构造函数
    for (const auto& it : v) {
        value.V_array->push_back(it);
    }
}

JsonObject::JsonObject(const std::map<std::string, JsonObject>& v) : type(ValueType::T_object) {
    value.V_object = new std::map<std::string, JsonObject>;
    for (const auto& pair : v) {
        value.V_object->emplace(pair.first, pair.second);
    }
}

JsonObject::JsonObject(const JsonObject& other) : type(other.type) {
    switch (type) {
        case ValueType::T_NULL:
            value.V_NULL = other.value.V_NULL;
            break;
        case ValueType::T_bool:
            value.V_bool = other.value.V_bool;
            break;
        case ValueType::T_int:
            value.V_int = other.value.V_int;
            break;
        case ValueType::T_double:
            value.V_double = other.value.V_double;
            break;
        case ValueType::T_string:
            value.V_string = new std::string(*other.value.V_string);  // 深拷贝
            break;
        case ValueType::T_array:
            value.V_array = new std::vector<JsonObject>(*other.value.V_array);
            break;
        case ValueType::T_object:
            value.V_object = new std::map<std::string, JsonObject>(*other.value.V_object);
            break;
        default:
            break;
    }
}

void JsonObject::clear() {
    if (value.V_NULL) {
        return;
    }
    switch (type) {
        case ValueType::T_string:
            delete value.V_string;
            value.V_string = nullptr;
            break;
        case ValueType::T_array:
            for (auto& it : *value.V_array) {
                it.clear();
            }
            delete value.V_array;
            value.V_array = nullptr;
            break;
        case ValueType::T_object:
            for (auto& pair : *value.V_object) {
                pair.second.clear();
            }
            delete value.V_object;
            value.V_object = nullptr;
            break;
        default:
            break;
    }
    value.V_NULL = true;
}

JsonObject& JsonObject::operator[](const std::string& key) const {
    if (type == ValueType::T_object) {
        return value.V_object->at(key);  // 传入的key必须存在，否则抛出异常
    } else {
        throw std::runtime_error("not an object");
    }
}
JsonObject& JsonObject::operator[](const int& index) const {
    if (type == ValueType::T_array) {
        return value.V_array->at(index);  // 传入的index必须存在，否则抛出异常
    } else {
        throw std::runtime_error("not an array");
    }
}
std::ostream& JObject::operator<<(std::ostream& os, const JsonObject& obj) {
    os << obj.toString();
    return os;
}
void JsonObject::erase(const std::string& key) {
    if (type == ValueType::T_object) {
        value.V_object->erase(key);
    } else {
        throw std::runtime_error("not an object");
    }
}
void JsonObject::erase(const int& index) {
    if (type == ValueType::T_array) {
        value.V_array->erase(value.V_array->begin() + index);
    } else {
        throw std::runtime_error("not an array");
    }
}

std::string JsonObject::toString() const {
    std::string res;
    switch (type) {
        case ValueType::T_NULL:
            return "null";
        case ValueType::T_bool:
            return value.V_bool ? "true" : "false";
        case ValueType::T_int:
            return std::to_string(value.V_int);
        case ValueType::T_double:
            res = std::to_string(value.V_double);
            for (auto it = res.end() - 1; it != res.begin(); it--) {
                if (*it == '0' && *(it - 1) != '.') {  // 最多允许保留一位‘0’
                    res.pop_back();
                } else {
                    break;
                }
            }
            return res;
        case ValueType::T_string:
            return ("\"" + *(value.V_string) + "\"");
        case ValueType::T_array:
            res += "[";
            for (auto& it : *(value.V_array)) {
                res += it.toString() + ",";
            }
            res.pop_back();
            res += "]";
            return res;
        case ValueType::T_object:
            res += "{";
            for (auto& pair : *(value.V_object)) {
                res += "\"" + pair.first + "\":" + pair.second.toString() + ",";
            }
            res.pop_back();
            res += "}";
            return res;
        default:
            break;
    }
    return "";
}

std::string JsonObject::getString() const {
    if (type == ValueType::T_string) {
        return *(value.V_string);
    }
    return "";
}
void JsonObject::out_to_file(const std::string& file_path, bool is_format) {  // 可选择是否格式化输出
    std::ofstream ofs(file_path, std::ios::out);
    std::string str = toString();
    int indentWidth = 4;
    int indent_count = 0;
    int bracket_index = -1;
    std::vector<std::string> indent_stack;
    for (int i = 0; i < 6; i++) {
        indent_stack.push_back(std::string(i * indentWidth, ' '));
    }
    if (is_format) {
        for (int i = 0; i < str.size(); i++) {
            switch (str[i]) {
                case '{':
                    indent_count++;
                    ofs << '{' << "\n" << indent_stack[indent_count];
                    break;
                case '}':
                    indent_count--;
                    ofs << "\n" << indent_stack[indent_count] << '}';
                    break;
                case '[':
                    bracket_index = i;
                    while (str[bracket_index] != ']') {
                        bracket_index++;
                        if (str[bracket_index] == '[') {
                            break;
                        }
                    }
                    if ((bracket_index - i >= 20) || (str[bracket_index] == '[')) {
                        indent_count++;
                        ofs << '[' << "\n" << indent_stack[indent_count];
                        break;
                    }
                    for (; str[i] != ']'; i++) {
                        ofs << str[i];
                        if (str[i] == ',') {
                            ofs << " ";
                        }
                    }
                    ofs << ']';
                    bracket_index = -1;
                    break;
                case ']':
                    indent_count--;
                    ofs << "\n" << indent_stack[indent_count] << ']';
                    break;
                case ',':
                    ofs << ',' << "\n" << indent_stack[indent_count];
                    break;
                case ':':
                    ofs << ':' << ' ';
                    break;
                default:
                    ofs << str[i];
                    break;
            }
        }
        ofs.close();
        return;
    }
    ofs << str << std::endl;
    ofs.close();
}