#include "mcc_json.hpp"




class UseJson
{
public:
    enum class JsonDataType : int
    {
        none = -1,
        age,
        height,
        name,
        agree,
        operation,
        vote,
        weight,
        price,
        call,
        english,
        korea,
        objectData
    };

    template <typename T>
    std::string EnumToString(T value)
    {
        switch (value)
        {
        case T::operation:
            return "Operation";
        case T::vote:
            return "VOTE";
        case T::weight:
            return "WEIGHT";
        case T::price:
            return "PRICE";
        case T::call:
            return "CALL";
        case T::english:
            return "ENGLISH";
        case T::korea:
            return "KOREA";
        case T::objectData:
            return "ObjectData";
        default:
            return "";
        }
    }


    TinyC::Json m_json{};

    UseJson() = default;
    ~UseJson() = default;

    bool GetValue_bool(JsonDataType type)
    {
        switch (type)
        {
        case JsonDataType::agree:
            // Json 데이터에서 "operationUseBuzzer" key의 bool 값 반환                
            return std::get<bool>(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return false;
        }
    }
    int GetValue_int(JsonDataType  type)
    {
        switch (type)
        {
        case JsonDataType::age:
            return std::get<int>(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return 0;
        }
    }
    double GetValue_real(JsonDataType  type)
    {
        switch (type)
        {
        case JsonDataType::height:
            return std::get<double>(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return 0.0;
        }
    }
    std::string GetValue_string(JsonDataType  type)
    {
        switch (type)
        {
        case JsonDataType::name:
            return std::get<std::string>(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return std::string{};
        }

    }

    std::vector<bool> GetValue_bools(JsonDataType type)
    {
        switch (type)
        {
        case JsonDataType::vote:
            return std::get<std::vector<bool>>(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return std::vector<bool>{};
        }
    }

    std::vector<int> GetValue_ints(JsonDataType type)
    {
        switch (type)
        {
        case JsonDataType::weight:
            return std::get<std::vector<int>>(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return std::vector<int>{};
        }
    }

    std::vector<double> GetValue_reals(JsonDataType type)
    {
        switch (type)
        {
        case JsonDataType::price:
            return std::get<std::vector<double>>(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return std::vector<double>{};
        }
    }

    std::vector<std::string> GetValue_strings(JsonDataType type)
    {
        switch (type)
        {
        case JsonDataType::call:
            return std::get<std::vector<std::string>>(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return std::vector<std::string>{};
        }
    }

    TinyC::json_obj_t GetValue_objs(JsonDataType type)
    {
        switch (type)
        {
        case JsonDataType::operation:
        case JsonDataType::objectData:
            return std::get<TinyC::json_obj_t >(m_json.m_datas[static_cast<int>(type)].value);

            // 나머지 case들에 대한 처리
        default:
            return TinyC::json_obj_t{};
        }
    }



    // GetData 함수
    template <typename T>
    T GetData(UseJson::JsonDataType type)
    {
        if constexpr (std::is_same_v<T, std::vector<int>>)
        {
            T value{};
            value = GetValue_ints(type);
            return value;
        }
        else if constexpr (std::is_same_v<T, std::vector<double>>)
        {
            T value{};
            value = GetValue_reals(type);
            return value;
        }
        else if constexpr (std::is_same_v<T, std::vector<bool>>)
        {
            T value{};
            value = GetValue_bools(type);
            return value;
        }
        else if constexpr (std::is_same_v<T, std::vector<std::string>>)
        {
            T value{};
            value = GetValue_strings(type);
            return value;
        }
        else if constexpr (std::is_same_v<T, TinyC::json_obj_t>)
        {
            T value{};
            value = GetValue_objs(type);
            return value;
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            return static_cast<T>(GetValue_string(type));
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return static_cast<T>(GetValue_bool(type));
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            return static_cast<T>(GetValue_int(type));
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            return static_cast<T>(GetValue_real(type));
        }
        else
            return T{};
    }

private:

public:
    inline bool LoadJsonFile(const std::string& filename)
    {
        return m_json.loadFromFile(filename);
    }

    inline bool SaveToFile(const std::string& filename) const
    {
        std::ofstream ofs(filename);
        if (!ofs.is_open())
            return false;

        ofs << m_json.toJsonString(); // toJsonString()은 json 객체를 json 형식의 문자열로 변환하는 함수입니다.
        ofs.close();

        return true;
    }

    inline std::string toJsonString() const {
        return m_json.toJsonString();
    }
};
// end of class UseJson

void use_json_class(void)
{

    constexpr const char *def_filename = "./data.json";

    UseJson use_json;

    // load file
    auto result = use_json.LoadJsonFile(def_filename);

    using ret_bools = std::vector<bool>;
    ret_bools ret1 = use_json.GetValue_bools(UseJson::JsonDataType::vote);

    for (const auto &elm : ret1)
    {
        std::cout << " elm 1 : " << elm << std::endl;
    }

    using ret_ints = std::vector<int>;
    ret_ints ret2 = use_json.GetValue_ints(UseJson::JsonDataType::weight);
    for (const auto &elm : ret2)
    {
        std::cout << " elm 2 : " << elm << std::endl;
    }

    using ret_reals = std::vector<double>;
    ret_reals ret3 = use_json.GetData<ret_reals>(UseJson::JsonDataType::price);
    for (const auto &elm : ret3)
    {
        std::cout << " elm 3 : " << elm << std::endl;
    }

    using ret_strs = std::vector<std::string>;
    ret_strs ret4 = use_json.GetData<ret_strs>(UseJson::JsonDataType::call);
    for (const auto &elm : ret4)
    {
        std::cout << " elm 4 : " << elm << std::endl;
    }

    using ret_obj = TinyC::json_obj_t;
    ret_obj ret5 = use_json.GetData<ret_obj>(UseJson::JsonDataType::operation);
    std::stringstream ss;
    for (const auto &[key, value] : ret5)
    {
        std::cout << " elm 5 : key [" << key << "] value [" << value.toJsonString() << "]" << std::endl;
        // auto& data = std::get<TinyC::json_t>(value);
    }

    using ret_obj2 = TinyC::json_obj_t;
    ret_obj2 ret6 = use_json.GetData<ret_obj2>(UseJson::JsonDataType::objectData);

    for (const auto &[key, value] : ret6)
    {
        std::cout << " elm 6 : key [" << key << "] value [" << value.toJsonString() << "]" << std::endl;
        // auto& data = std::get<TinyC::json_t>(value);
    }

    // write file
    use_json.SaveToFile("./copy.json");

    std::cout << use_json.toJsonString() << std::endl;
}

int main()
{
    std::cout << "test start" << std::endl;

    use_json_class();


    return 0;
}