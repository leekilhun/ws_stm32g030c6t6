/*******************************************************************
 * \file   sys_data.hpp
 * \brief  �ý��ۿ� ����� �����͸� �����ϰ� �� ��ü���� �̿��� �� �ֵ��� api�� �����Ѵ�.
 * 
 * \author gns2.lee (gns2.lee@samsung.com)
 * \date   March 2024
 * 
 * 
 * @copyright Copyright (c) 2024
 * 
 *********************************************************************/
/*


[
    // json ���� �ۼ� ��Ģ
    // 1. ������ �ݵ�� { �� �����ϸ� �������� } �� ������ �Ѵ�.
    // 2. { } ���̿��� "key" : json_t ���·� �ۼ��Ѵ�.

    // pasing flow
    // 1. { } ������ ������ �Ľ��Ѵ�.
    // 2. "key" : json_t ���·� �Ľ��Ѵ�. ����Ű�� ����
    // 3. objects�� �����Ѵ�.
    // 4. objects�� type�� object�� ���� �ٽ� value�� �Ľ��Ѵ�.
    //    object - map<string, json_t>
    // 5. array [json_t, json_t, ...] Ÿ����

    // type object { "key" : json_t, "key" : json_t, ... }
    // type array [json_t, json_t, ...]



 
*/

#include "sys_def.hpp"


namespace ap_core 
{
  class ConfigData
  {

  public:
    /**
     * json �������� key data ������� �ۼ��Ѵ�.
     */
    enum class data_reg_idx : int
    {
      none = -1,
      serial_no,
      machine,
      approve,
      digit,
      numbers,
      checks,
      members,
      common,
      operation,
      serial_comm,
      socket_net,
      configs,
      _max,
    };

    template <typename T>
      requires std::is_same_v<T, data_reg_idx>
    std::string EnumToString(T value)
    {
      switch (value)
      {
      case T::serial_no:
        return "serial_no";
      case T::machine:
        return "machine";
      case T::approve:
        return "approve";
      case T::digit:
        return "digit";
      case T::numbers:
        return "numbers";
      case T::checks:
        return "checks";
      case T::members:
        return "members";
      case T::common:
        return "common";
      case T::operation:
        return "operation";
      case T::serial_comm:
        return "serial_comm";
      case T::socket_net:
        return "socket_net";
      case T::configs:
        return "configs";
      default:
        return "";
      }
    }

    TinyC::Json m_json{};

    ConfigData() = default;
    ~ConfigData() = default;
    bool m_isOpened{};
    std::string m_fileinfo{};

    // copy constructor
    ConfigData(const ConfigData& other) = default;
    // copy assignment
    ConfigData& operator=(const ConfigData& other) = default;
    // move constructor
    ConfigData(ConfigData&& other) = default;
    // move assignment
    ConfigData& operator=(ConfigData&& other) = default;


  public:

    TinyC::json_obj_t make_json_obj(data_reg_idx idx, TinyC::json_t value)
    {
      TinyC::json_obj_t ret{};
      ret.emplace(EnumToString(idx), value);
      return ret;      
    }

    bool GetValue_bool(data_reg_idx type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return false;
      }

      switch (type)
      {
      case data_reg_idx::approve:
        return std::get<bool>(m_json.m_objects[static_cast<int>(type)]["approve"].value);
        // ������ case�鿡 ���� ó��
      default:
        return false;
      }
    }

    int GetValue_int(data_reg_idx  type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return 0;
      }
      switch (type)
      {
      case data_reg_idx::serial_no:
        return std::get<int>(m_json.m_objects[static_cast<int>(type)]["serial_no"].value);

        // ������ case�鿡 ���� ó��
      default:
        return 0;
      }
    }

    double GetValue_real(data_reg_idx  type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return 0.0;
      }
      switch (type)
      {
      case data_reg_idx::digit:
        return std::get<double>(m_json.m_objects[static_cast<int>(type)]["digit"].value);
        // ������ case�鿡 ���� ó��
      default:
        return 0.0;
      }
    }
    std::string GetValue_string(data_reg_idx  type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return std::string{};
      }
      switch (type)
      {
      case data_reg_idx::machine:
        return std::get<std::string>(m_json.m_objects[static_cast<int>(type)]["machine"].value);
        // ������ case�鿡 ���� ó��
      default:
        return std::string{};
      }

    }

    std::vector<bool> GetValue_bools(data_reg_idx type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return std::vector<bool>{};
      }
      switch (type)
      {
			case data_reg_idx::checks:
				return std::get<std::vector<bool>>(m_json.m_objects[static_cast<int>(type)]["checks"].value);
        // ������ case�鿡 ���� ó��
      default:
        return std::vector<bool>{};
      }
    }

    std::vector<int> GetValue_ints(data_reg_idx type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return std::vector<int>{};
      }
      switch (type)
      {
      case data_reg_idx::numbers:
				return std::get<std::vector<int>>(m_json.m_objects[static_cast<int>(type)]["numbers"].value);

        // ������ case�鿡 ���� ó��
      default:
        return std::vector<int>{};
      }
    }

    std::vector<double> GetValue_reals(data_reg_idx type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return std::vector<double>{};
      }
      //switch (type)
      //{
      //case data_reg_idx::weight:
      //  return std::get<std::vector<double>>(m_json.m_objects[static_cast<int>(type)]["weight"].value);
        // ������ case�鿡 ���� ó��
      //default:
        return std::vector<double>{};
      //}
    }

    std::vector<std::string> GetValue_strings(data_reg_idx type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return std::vector<std::string>{};
      }
      switch (type)
      {
        case data_reg_idx::members:
             return std::get<std::vector<std::string>>(m_json.m_objects[static_cast<int>(type)]["members"].value);

        // ������ case�鿡 ���� ó��
      default:
        return std::vector<std::string>{};
      }
    }

    TinyC::json_obj_t GetValue_objs(data_reg_idx type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return TinyC::json_obj_t{};
      }
      switch (type)
      {
      case data_reg_idx::operation:
      case data_reg_idx::common:
      case data_reg_idx::serial_comm:
      case data_reg_idx::socket_net:
      case data_reg_idx::configs:
        //return std::get<TinyC::json_obj_t >(m_json.m_datas[static_cast<int>(type)].value);
        return m_json.m_objects[static_cast<int>(type)];
        // ������ case�鿡 ���� ó��
      default:
        return TinyC::json_obj_t{};
      }
    }

   
    // GetData �Լ�
    template <typename T>      
    T GetData(data_reg_idx type)
    {
      if (m_isOpened == false)
      {
        LOG_PRINT("No a file loaded");
        return T{};
      }
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




    public:
      inline bool LoadJsonFile(const std::string& filename)
      {
        m_isOpened = m_json.loadFromFile(filename);
        m_fileinfo = filename;
        return m_isOpened;
      }

      inline bool SaveToFile()
      {
        if (m_isOpened == false)
        {
          LOG_PRINT("No a file loaded");
          return false;
        }  
        return m_json.saveToFile(m_fileinfo);
      }

      inline std::string toJsonString() const {
        if (m_isOpened == false)
        {
          LOG_PRINT("No a file loaded");
          return  std::string{};
        }
        return m_json.toJsonString();
      }



  };
  // tiny_jsonC
}
// end of namespace ap_core
