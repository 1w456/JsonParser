/*
 * 因为一个json文件实际只有一个元素，通过嵌套实现存储数据
 * 所以可以用一个嵌套的C++类来表示一个json对象
 * 的
 */
#ifndef _JOBJECT_H_
#define _JOBJECT_H_

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <windows.h>

namespace JObject {
    // 前置声明JsonObject类，避免循环引用
    class JsonObject;
    std::ostream& operator<<(std::ostream& os, const JsonObject& obj);
    // 使用联合体表示json支持的6种值类型
    union Value {
        bool V_NULL;
        bool V_bool;
        int V_int;
        double V_double;
        std::string* V_string;
        std::vector<JsonObject>* V_array;
        std::map<std::string, JsonObject>* V_object;
    };
    // 枚举类型表示json的值类型
    // C++11后支持强类型枚举，必须用枚举名::枚举值访问，且不会隐式转换为int类型，更加安全
    enum class ValueType {
        T_NULL,  // 用于标识对象是否被释放
        T_bool,
        T_int,
        T_double,
        T_string,
        T_array,
        T_object
    };

    class JsonObject {
      private:
        Value value{false};
        ValueType type;

      public:
        // 对于除了基本类型（比如int、float等）的变量，不使用成员初始化列表时会多一次默认初始化过程
        JsonObject() : type(ValueType::T_NULL) {}
        JsonObject(bool v) : type(ValueType::T_bool) { value.V_bool = v; }
        JsonObject(int v) : type(ValueType::T_int) { value.V_int = v; }
        JsonObject(double v) : type(ValueType::T_double) { value.V_double = v; }
        JsonObject(const std::string& v);
        JsonObject(const char* v);
        JsonObject(const std::vector<JsonObject>& v);
        JsonObject(const std::map<std::string, JsonObject>& v);
        JsonObject(const JsonObject& other);
        ~JsonObject() { clear(); }
        void clear();
        JsonObject& operator[](const std::string& key) const;
        JsonObject& operator[](const int& index) const;
        void erase(const std::string& key);
        void erase(const int& index);
        std::string getString() const;
        std::string toString() const;
        void out_to_file(const std::string& file_path, bool is_format = true);
    };
}  // namespace JObject
#endif