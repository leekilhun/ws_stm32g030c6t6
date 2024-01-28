#pragma once
#ifndef _COMMON_MCC_JSON_HPP
#define _COMMON_MCC_JSON_HPP

/**
 * @file mcc_json.hpp
 * @author lee kil hun (gns.lee@samsung.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../def.hpp"

namespace TinyC
{
    struct json_t;

    typedef std::map<std::string, json_t> json_obj_t;

    struct json_t
    {
    public:
        enum class valueType : int
        {
            none,
            object,
            array,
            string,
            integer,
            real,
            boolean,
        } type = valueType::none;

        using variant_t = std::variant<std::monostate, json_obj_t,
                                       std::vector<std::string>, std::vector<int>, std::vector<double>, std::vector<bool>,
                                       std::string, int, double, bool>;

        variant_t value;

        json_t() = default;

        // copy constructor
        json_t(const json_t &other) : type(other.type), value(other.value) {}

        // move constructor
        json_t(json_t &&other) noexcept : type(std::exchange(other.type, valueType::none)), value(std::move(other.value)) {}

        // copy assignment operator
        json_t &operator=(const json_t &other)
        {
            type = other.type;
            value = other.value;
            return *this;
        }

        // move assignment operator
        json_t &operator=(json_t &&other) noexcept
        {
            type = std::exchange(other.type, valueType::none);
            value = std::move(other.value);
            return *this;
        }

        size_t valueIndex() const
        {
            return value.index();
        }

        template <typename T>
        json_t operator=(const T &value)
        {
            type = getType(value);
            this->value = value;
            return *this;
        }

        template <typename T>
        valueType getType(const T &value)
        {
            if constexpr (std::is_same_v<T, std::monostate>)
            {
                return valueType::none;
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return valueType::string;
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return valueType::boolean;
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                return valueType::integer;
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return valueType::real;
            }
            else if constexpr (std::is_same_v<T, std::vector<std::string>>)
            {
                return valueType::array;
            }
            else if constexpr (std::is_same_v<T, std::vector<int>>)
            {
                return valueType::array;
            }
            else if constexpr (std::is_same_v<T, std::vector<double>>)
            {
                return valueType::array;
            }
            else if constexpr (std::is_same_v<T, std::vector<bool>>)
            {
                return valueType::array;
            }
            else if constexpr (std::is_same_v<T, json_obj_t>)
            {
                return valueType::object;
            }
            else
            {
                throw std::runtime_error("Unsupported type for json_t");
                return valueType::none;
            }
        }

        inline std::string toJsonString() const
        {

            std::stringstream ss;
            switch (type)
            {
            case valueType::none:
                ss << "null";
                break;
            case valueType::object:
            {
                ss << "{ \n";
                const auto &obj = std::get<json_obj_t>(value);
                bool first = true;
                for (const auto &[key, value] : obj)
                {
                    if (!first)
                    {
                        ss << " , ";
                    }
                    ss << "    \"" << key << "\" : " << value.toJsonString();
                    first = false;
                }
                ss << "\n}";
                break;
            }
            case valueType::array:
            {
                ss << "[ \n";

                switch ((valueType)value.index())
                {
                case valueType::object:
                {
                    auto &arr = std::get<json_obj_t>(value);
                    bool is_first = true;
                    for (const auto &elem : arr)
                    {
                        if (!is_first)
                        {
                            ss << " , \n";
                        }
                        ss << "    {"
                           << "\"" << elem.first << "\" : ";
                        ss << elem.second.toJsonString() << "} ";
                        is_first = false;
                    }
                }
                break;
                case valueType::string:
                {
                    bool first = true;
                    auto &values = std::get<std::vector<std::string>>(value);
                    for (const auto &elem : values)
                    {
                        if (!first)
                        {
                            ss << " ,    \n";
                        }
                        ss << "    \"" << elem << "\"";
                        first = false;
                    }
                }
                break;
                case valueType::integer:
                {
                    bool first = true;
                    auto &values = std::get<std::vector<int>>(value);
                    for (const auto &elem : values)
                    {
                        if (!first)
                        {
                            ss << " ,    \n";
                        }
                        ss << "    " << elem;
                        first = false;
                    }
                }
                break;
                case valueType::real:
                {
                    bool first = true;
                    auto &values = std::get<std::vector<double>>(value);
                    for (const auto &elem : values)
                    {
                        if (!first)
                        {
                            ss << " ,    \n";
                        }
                        ss << "    " << elem;
                        first = false;
                    }
                }
                break;
                case valueType::boolean:
                {
                    bool first = true;
                    auto &values = std::get<std::vector<bool>>(value);
                    for (const auto &elem : values)
                    {
                        if (!first)
                        {
                            ss << " ,    \n";
                        }
                        ss << "    " << (elem ? "true" : "false");
                        first = false;
                    }
                }
                break;
                default:
                    break;
                }
                ss << "\n    ]";
                break;
            }
            case valueType::string:
                ss << "\"" << std::get<std::string>(value) << "\"";
                break;
            case valueType::integer:
                ss << std::get<int>(value);
                break;
            case valueType::real:
                ss << std::get<double>(value);
                break;
            case valueType::boolean:
                ss << (std::get<bool>(value) ? "true" : "false");
                break;
            }
            return ss.str();
        }
    };

    class Json
    {
    public:
        Json() = default;
        ~Json() = default;

        std::vector<json_obj_t> m_objects;
        std::vector<json_t> m_datas;
        inline bool loadFromFile(const std::string &filename)
        {
            std::ifstream inFile(filename);
            if (!inFile.is_open())
            {
                return false;
            }

            std::string str((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            return parse(str);
        }

        inline bool saveToFile(const std::string &filename) const
        {
            std::ofstream ofs(filename);
            if (!ofs.is_open())
                return false;

            ofs << toJsonString(); // toJsonString()은 json 객체를 json 형식의 문자열로 변환하는 함수입니다.
            ofs.close();

            return true;
        }

        inline std::string toJsonString() const
        {
            std::stringstream ss;

            ss << "{ \n";
            using jvtype = json_t::valueType;

            for (const auto &data : this->m_objects)
            {
                if (data.empty())
                    return std::string{};
                if (data.size() == 1)
                {
                    auto elm = data.begin();
                    std::string key = elm->first;
                    ss << "  \"" << key << "\" : ";
                    json_t value = elm->second;
                    switch (value.type)
                    {
                    case json_t::valueType::none:
                        ss << "null";
                        break;
                    case json_t::valueType::object:
                    {
                        ss << "{ \n";
                        auto &obj = std::get<json_obj_t>(value.value);
                        bool first = true;
                        for (const auto &[key, value] : obj)
                        {
                            if (!first)
                            {
                                ss << ", \n";
                            }
                            ss << "    \"" << key << "\" : " << value.toJsonString();
                            first = false;
                        }
                        ss << "\n  }";
                    }
                    break;
                    case json_t::valueType::array:
                    {
                        ss << "[ \n";

                        enum class arry_type : int
                        {
                            object = 1,
                            string,
                            integer,
                            real,
                            boolean,
                        };

                        switch ((arry_type)value.valueIndex())
                        {
                        case arry_type::object:
                        {
                            auto &arr = std::get<json_obj_t>(value.value);
                            bool first = true;
                            for (const auto &elem : arr)
                            {
                                if (!first)
                                {
                                    ss << ", \n";
                                }
                                ss << "    {"
                                   << "\"" << elem.first << "\" : ";
                                ss << elem.second.toJsonString() << "}";
                                first = false;
                            }
                        }
                        break;
                        case arry_type::string:
                        {
                            bool first = true;
                            auto &values = std::get<std::vector<std::string>>(value.value);
                            for (const auto &elem : values)
                            {
                                if (!first)
                                {
                                    ss << " ,    \n";
                                }
                                ss << "    \"" << elem << "\"";
                                first = false;
                            }
                        }
                        break;
                        case arry_type::integer:
                        {
                            bool first = true;
                            auto &values = std::get<std::vector<int>>(value.value);
                            for (const auto &elem : values)
                            {
                                if (!first)
                                {
                                    ss << " ,    \n";
                                }
                                ss << "    " << elem;
                                first = false;
                            }
                        }
                        break;
                        case arry_type::real:
                        {
                            bool first = true;
                            auto &values = std::get<std::vector<double>>(value.value);
                            for (const auto &elem : values)
                            {
                                if (!first)
                                {
                                    ss << " ,    \n";
                                }
                                ss << "    " << elem;
                                first = false;
                            }
                        }
                        break;
                        case arry_type::boolean:
                        {
                            bool first = true;
                            auto &values = std::get<std::vector<bool>>(value.value);
                            for (const auto &elem : values)
                            {
                                if (!first)
                                {
                                    ss << " ,    \n";
                                }
                                ss << "    " << (elem ? "true" : "false");
                                first = false;
                            }
                        }
                        break;
                        default:
                            break;
                        }

                        ss << "\n  ], \n";
                    }
                    break;
                    case json_t::valueType::string:
                        ss << "\"" << std::get<std::string>(value.value) << "\", \n";
                        break;
                    case json_t::valueType::integer:
                        ss << std::get<int>(value.value) << ", \n";
                        break;
                    case json_t::valueType::real:
                        ss << std::get<double>(value.value) << ", \n";
                        break;
                    case json_t::valueType::boolean:
                        ss << (std::get<bool>(value.value) ? "true" : "false") << ", \n";
                        break;
                    default:
                        break;
                    }
                }
                else
                {

                    for (const auto &[key, value] : data)
                    {
                        ss << "\"" << key << "\":" << value.toJsonString();
                    }
                }
            }

            ss << "\n } \n";
            return ss.str();
        }

        inline void trim(std::string &str)
        {
            // 앞에서부터 공백문자 제거
            std::size_t start = str.find_first_not_of(" \t\r\n");
            if (start != std::string::npos)
            {
                str = str.substr(start);
            }
            else
            {
                str.clear();
                return;
            }

            // 뒤에서부터 공백문자 제거
            std::size_t end = str.find_last_not_of(" \t\r\n");
            if (end != std::string::npos)
            {
                str = str.substr(0, end + 1);
            }
            else
            {
                str.clear();
            }
        }

        inline const char *find_first_char(const std::string &str, char ch)
        {
            std::size_t i = 0; //
            std::size_t len = str.length();
            bool next_comma = false; // 쉼표 다음에 값이 나올 예정인지 확인하기 위한 플래그
            // 배열 값 파싱 루프
            while (i < len)
            {
                // 공백문자 제거
                while (i < len && (str[i] == ' ' || str[i] == '\t' || str[i] == '\r'))
                {
                    ++i;
                }
                if (i == len)
                {
                    break;
                }

                //  주석 줄 제거
                if (str[i] == '/' && str[i + 1] == '/')
                {
                    i += 2;
                    while (i < len && str[i] != '\n')
                    {
                        i++;
                    }
                }

                // ch와 일치하는 문자를 찾으면 해당 위치를 반환
                if (str[i] == ch)
                {
                    return &str[i];
                }

                i++;
            }
            return nullptr;
        }

        inline const char *find_last_char(const std::string &str, char ch)
        {
            size_t pos = str.rfind(ch);
            if (pos == std::string::npos)
            {
                // 문자가 문자열에 없는 경우
                return nullptr;
            }
            else
            {
                // 문자가 문자열에 있는 경우
                return &str[pos];
            }
        }

        inline int replace(std::string &str, const char *before, const char *after)
        {
            int count = 0;
            std::size_t pos = 0;
            while ((pos = str.find(before, pos)) != std::string::npos)
            {
                str.replace(pos, strlen(before), after);
                pos += strlen(after);
                count++;
            }
            return count;
        }

        inline int replace_trim(std::string &str, const char *before, const char *after)
        {
            int count = 0;
            std::size_t pos = 0;
            while ((pos = str.find(before, pos)) != std::string::npos)
            {
                std::size_t start_pos = pos;
                std::size_t end_pos = pos + strlen(before);

                // Remove leading whitespace characters
                while (start_pos > 0 && std::isspace(str[start_pos - 1]))
                {
                    start_pos--;
                }

                // Remove trailing whitespace characters
                while (end_pos < str.length() && std::isspace(str[end_pos]))
                {
                    end_pos++;
                }

                // Replace the substring
                str.replace(start_pos, end_pos - start_pos, after);
                pos = start_pos + strlen(after);

                count++;
            }
            return count;
        }

        inline std::string_view trim(std::string_view str)
        {
            // 공백문자 집합
            constexpr std::string_view whitespace{" \t\n\r\f\v"};

            // 앞쪽의 공백문자 제거
            const auto firstNonWhitespace = str.find_first_not_of(whitespace);
            str.remove_prefix(firstNonWhitespace);

            // 뒤쪽의 공백문자 제거
            const auto lastNonWhitespace = str.find_last_not_of(whitespace);
            if (lastNonWhitespace != std::string_view::npos)
            {
                str.remove_suffix(str.size() - lastNonWhitespace - 1);
            }

            return str;
        }

        inline bool isInteger(const std::string_view &str)
        {
            if (str.empty())
            {
                return false;
            }

            std::size_t i = 0;
            if (str[i] == '+' || str[i] == '-')
            {
                i++;
            }

            if (i >= str.length())
            {
                return false;
            }

            for (; i < str.length(); i++)
            {
                if (!std::isdigit(str[i]))
                {
                    return false;
                }
            }

            return true;
        }
        inline bool isBool(const std::string_view &str)
        {
            if (str.empty())
            {
                return false;
            }

            std::size_t i = 0;
            for (; i < str.length(); i++)
            {
                if (str[i] == 'f' || str[i] == 'F' || str[i] == 't' || str[i] == 'T')
                {
                    return true;
                }
            }
            return false;
        }

        inline bool isReal(const std::string_view &str)
        {
            if (str.empty())
            {
                return false;
            }

            std::size_t i = 0;
            if (str[i] == '+' || str[i] == '-')
            {
                i++;
            }

            bool hasDecimal = false;
            bool hasExponent = false;
            for (; i < str.length(); i++)
            {
                if (std::isdigit(str[i]))
                {
                    continue;
                }
                else if (str[i] == '.')
                {
                    if (hasDecimal || hasExponent)
                    {
                        return false;
                    }
                    hasDecimal = true;
                }
                else if (str[i] == 'e' || str[i] == 'E')
                {
                    if (hasExponent)
                    {
                        return false;
                    }
                    hasExponent = true;
                    i++;
                    if (i >= str.length())
                    {
                        return false;
                    }
                    if (str[i] == '+' || str[i] == '-')
                    {
                        i++;
                    }
                    if (i >= str.length() || !std::isdigit(str[i]))
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }

            return hasDecimal || hasExponent || std::isdigit(str.back());
        }

        inline bool parseObject(std::string_view str, json_t &data, std::size_t &ret_idx)
        {
            std::size_t i{};

            auto get_number = [&str, &i](bool &is_double) -> std::string
            {
                std::size_t cnt{};
                std::stringstream ss{};
                while (i < str.size())
                {
                    char c = str[i];
                    if (c == '.')
                        is_double = true;
                    if (c == ',' || c == '}')
                        return ss.str();
                    ss << c;
                    i++;
                }
                return ss.str();
            };

            auto get_string = [&str, &i](auto find_c) -> std::string
            {
                i++; // " next
                std::string ret;
                while (i < str.size())
                {
                    char c = str[i];
                    if (c == '/')
                    {
                        if (str[++i] == '/')
                        {
                            i += 1;
                            while (str[i] != '\n')
                            {
                                i++;
                            }
                            i += 1;
                            c = ' ';
                        }
                    }

                    if (c == find_c)
                        return ret;
                    ret.push_back(c);
                    i++;
                }
                return ret;
            };

            auto get_end = [&str, &i]() -> std::size_t
            {
                while (i < str.size())
                {
                    char c = str[i];
                    if (c == ',' || c == '}')
                        return i;
                    i++;
                }
                return i;
            };

            auto get_key = [&str, &i]() -> std::string
            {
                std::string ret{};
                while (i < str.size())
                {
                    char c = str[i];
                    if (c == '\"')
                    {
                        while (i < str.size())
                        {
                            ret.push_back(str[++i]);
                            if (str[i + 1] == '\"')
                            {
                                i = i + 2;
                                while (i < str.size())
                                {
                                    if (str[i++] == ':')
                                        return ret;
                                }
                            }
                        }
                    }
                    i++;
                }
                return ret;
            };

            while (1)
            {
                char c = str[i];
                if (c == ',' || c == '}')
                    break;

                switch (c)
                {
                case '{':
                {
                    json_obj_t obj{};
                    while (i < str.size())
                    {
                        std::string key = get_key();
                        if (key.empty())
                            break;
                        json_t ret_data{};
                        if (parseObject(&str[i + 1], ret_data, i))
                        {
                            obj.emplace(key, ret_data);
                        }
                        i++;
                    }
                    // while
                    ret_idx = ret_idx + i;
                    if (obj.size() != 0)
                    {
                        data = obj;
                        return true;
                    }
                }
                break;

                case '[':
                {
                    data.type = json_t::valueType::array;
                    std::string str = get_string(']');
                    ret_idx = ret_idx + i + 1;

                    json_t ret_data;
                    parseArray(str, ret_data);
                    data.value = ret_data.value;
                    return true;
                }
                break;

                case '"':
                {
                    data = get_string('"');
                    ret_idx = ret_idx + get_end();
                    return true;
                }
                break;

                case 't':
                case 'T':
                case 'f':
                case 'F':
                {
                    data = (c == 't' || c == 'T') ? true : false;
                    ret_idx = ret_idx + get_end();
                    return true;
                }
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
                case '+':
                {
                    bool is_double{};
                    std::string cut_str = get_number(is_double);
                    if (is_double)
                    {
                        data = std::stod((char *)cut_str.c_str());
                    }
                    else
                    {
                        data = std::stoi((char *)cut_str.c_str());
                    }
                    ret_idx = ret_idx + i;
                    return true;
                }
                break;

                default:
                    break;
                }
                // switch
                i++;
            }
            // while (1)

            return false;
        }

        inline int split(char *arg_str, char **argv, const char *delim_chars, size_t max = 1024)
        {
            int argc = 0;
            char *tok = nullptr;
            char *next_ptr = nullptr;

            for (tok = strtok_s(arg_str, delim_chars, &next_ptr); tok; tok = strtok_s(NULL, delim_chars, &next_ptr))
            {
                argv[argc++] = tok;
                if (argc >= max)
                    break;
            }
            return argc;
        }
        inline bool parseArray(std::string_view array_str, json_t &ret_data /*, std::size_t& ret_idx*/)
        {
            if (array_str.empty())
                return false;

            auto get_key = [](const std::string &str, std::size_t &idx) -> std::string
            {
                std::string ret{};
                while (idx < str.size())
                {
                    char c = str[idx];
                    if (c == '\"')
                    {
                        while (idx < str.size())
                        {
                            ret.push_back(str[++idx]);
                            if (str[idx + 1] == '\"')
                            {
                                idx = idx + 2;
                                while (idx < str.size())
                                {
                                    if (str[idx++] == ':')
                                        return ret;
                                }
                            }
                        }
                    }
                    idx++;
                }
                return ret;
            };

            const char delim_chars[] = ",";
            constexpr int elem_max = 256;
            char *argv[elem_max] = {};
            bool is_object = (array_str.find('{') != std::string::npos && array_str.find('}') != std::string::npos);
            int argc = split((char *)array_str.data(), argv, delim_chars, elem_max);

            if (is_object)
            {
                /*
                { "USE_BUZZER": false },       { "USE_DOOR": false },
                { "USE_MES": false },          { "SHIFT_A_TIME": 7 },
                { "SHIFT_C_TIME": 21 },        { "RETAY_MAX_CNT": 3 }
                */
                json_obj_t obj;
                ret_data.type = json_t::valueType::object;
                for (int i = 0; i < argc; i++)
                {
                    if (argv[i])
                    {
                        std::string obj_str(argv[i]);
                        std::size_t end_key{};
                        std::string key = get_key(obj_str, end_key);
                        json_t value{};
                        std::size_t idx = 0;
                        if (parseObject(&obj_str[end_key + 1], value, idx))
                            obj.emplace(key, value);
                        else
                            return false;
                    }
                    // ret_idx++;
                }
                // for  (int i = 0; i < argc; i++)
                ret_data.value = obj;
                return true;
            }
            else
            {
                /*
                false, true, false ,

                */
                if (argv[0])
                {
                    std::string_view value(trim(argv[0]));
                    if (isInteger(value))
                    {
                        ret_data.type = json_t::valueType::integer;
                        std::vector<int> v_datas;
                        for (int i = 0; i < argc; i++)
                        {
                            std::string_view value(trim(argv[i]));
                            if (isInteger(value))
                                v_datas.emplace_back(std::stoi(std::string(value)));
                            else
                                return false;

                            // ret_idx++;
                        }
                        // for
                        ret_data.value = v_datas;
                        return true;
                    }
                    else if (isReal(value))
                    {
                        ret_data.type = json_t::valueType::real;
                        std::vector<double> v_datas;
                        for (int i = 0; i < argc; i++)
                        {
                            std::string_view value(trim(argv[i]));
                            if (isReal(value))
                                v_datas.emplace_back(std::stod(std::string(value)));
                            else
                                return false;

                            // ret_idx++;
                        }
                        // for
                        ret_data.value = v_datas;
                        return true;
                    }
                    else if (value.front() == '"' && value.back() == '"')
                    {
                        ret_data.type = json_t::valueType::string;
                        std::vector<std::string> v_datas;
                        for (int i = 0; i < argc; i++)
                        {
                            std::string str(trim(argv[i]));
                            trim(str);
                            str.erase(0, 1);              // 맨 앞의 " 제거
                            str.erase(str.size() - 1, 1); // 맨 뒤의 " 제거
                            v_datas.emplace_back(str);
                            // ret_idx++;
                        }
                        // for
                        ret_data.value = v_datas;
                        return true;
                    }
                    else if (isBool(value))
                    {
                        ret_data.type = json_t::valueType::boolean;
                        std::vector<bool> v_datas;
                        for (int i = 0; i < argc; i++)
                        {
                            std::string_view value(trim(argv[i]));
                            if (value == "true" || value == "TRUE")
                                v_datas.emplace_back(true);
                            else
                                v_datas.emplace_back(false);
                            // ret_idx++;
                        }
                        // for
                        ret_data.value = v_datas;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                // if (argv[0])
            }
            // if (is_object)

            return false;
        }

        inline bool parse(const std::string &arrayString)
        {

            auto get_str = [&](auto &str) -> std::string
            {
                char *start_addr = strchr(str, '{');
                char *end_addr = strchr(str, '}');
                size_t length = (end_addr - start_addr) - 1;
                std::string ret(start_addr + 1, length);

                return ret;
            };

            if (arrayString.empty())
            {
                return false;
            }

            /*
             Json parses a single { ///// } object and stores it as data.
             {  /////  }  only one
            */

            char *start_addr = (char *)find_first_char(arrayString, '{');
            char *end_addr = (char *)find_last_char(arrayString, '}');
            size_t start_idx = start_addr - (char *)arrayString.c_str();
            size_t length = end_addr - start_addr;
            // arrayString[i]위치부터 end_addr까지의 문자열을 obj_str에 저장
            std::string obj_str = arrayString.substr(start_idx, length);

            // 배열 값 파싱 루프
            std::size_t pos = 1;
            std::size_t end_key{};

            while (pos < length)
            {
                end_key = obj_str.find(':', pos);
                if (end_key == std::string_view::npos)
                {
                    return false;
                }
                // key 문자에서 앞뒤로 " 제거
                std::string key(obj_str.data() + pos, end_key - pos);
                pos = +(replace_trim(key, "\"", ""));
                if (key.empty())
                {
                    return false;
                }

                pos = end_key + 1;
                json_t ret_object{};
                std::string_view view_str(&obj_str[pos]);
                if (parseObject(&obj_str[pos], ret_object, pos))
                {
                    json_obj_t obj{};
                    obj.emplace(key, ret_object);
                    m_objects.emplace_back(obj);
                    m_datas.emplace_back(ret_object);
                }
                else
                {
                    return false;
                }
                pos++;
            }
            // while (pos < length)

            if (m_objects.size() > 0)
                return true;
            else
                return false;
        }
    };
    // end of class Json
}
// end of namespace TinyC

#endif // _COMMON_MCC_LOG_HPP