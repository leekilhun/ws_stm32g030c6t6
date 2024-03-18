/*****************************************************************//**
 * \file   mcc_json.hpp
 * \brief
 *   3/10: 수정
 * \author gns2.lee (gns2.lee@samsung.com)
 * \date   March 2024
 *
 *
 * @copyright Copyright (c) 2024
 *
 *********************************************************************/


#pragma once
#ifndef _COMMON_MCC_JSON_HPP
#define _COMMON_MCC_JSON_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <type_traits>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <exception>
#include <variant>
#include <tuple>
#include <array>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <list>
#include <any>
#include <functional>
#include <future>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>
#include <iterator>
#include <execution>
#include <ratio>
#include <optional>
#include <string_view>
#include <memory_resource>
#include <fstream>


#ifndef NOMINMAX
#define NOMINMAX
#endif



namespace TinyC
{
	struct json_t;

	typedef std::map<std::string, json_t> json_obj_t;

	namespace types
	{

		/*template <typename Param>
		concept json_obj_c = std::same_as<Param, json_obj_t>;*/
	}
	//end of type

	/**
	 * json data 타입 .
	 */
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

		variant_t value{};

		json_t() = default;
		json_t(const variant_t& val) {
			value = val;
			type = get_varidx(val.index());
		};

		// copy constructor
		json_t(const json_t& other) : type(other.type), value(other.value) {}

		// move constructor
		json_t(json_t&& other) noexcept : type(std::exchange(other.type, valueType::none)), value(std::move(other.value)) {}

		// copy assignment operator
		json_t& operator=(const json_t& other)
		{
			type = other.type;
			value = other.value;
			return *this;
		}

		// move assignment operator
		json_t& operator=(json_t&& other) noexcept
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
		json_t operator=(const T& value)
		{
			type = getType(value);
			this->value = value;
			return *this;
		}

		template<typename T>
		static bool is_obj_type(const T& value)
		{
			if constexpr (std::is_same_v<T, json_obj_t>)
			{
				return true;
			}
			return false;
		}

		template <typename T>
		valueType getType(const T& value)
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
			else if constexpr (std::is_same_v<T, json_t>)
			{
				return valueType::object;
			}
			else
			{
				throw std::runtime_error("Unsupported type for json_t");
				return valueType::none;
			}
		}


		inline static valueType get_varidx(size_t idx)
		{

			enum { _none, _jsonobj, _strings, _ints, _reals, _bools, _str, _int, _real, _bool };
			switch (idx)
			{

			case _jsonobj:
				return valueType::object;
			case _strings:
				return valueType::array;
			case _ints:
				return valueType::array;
			case _reals:
				return valueType::array;
			case _bools:
				return valueType::array;
			case _str:
				return valueType::string;
			case _int:
				return valueType::integer;
			case _real:
				return valueType::real;
			case _bool:
				return valueType::boolean;
			case _none:
			default:
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
				const auto& obj = std::get<json_obj_t>(value);
				bool first = true;
				for (const auto& [key, value] : obj)
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
					auto& arr = std::get<json_obj_t>(value);
					bool is_first = true;
					for (const auto& elem : arr)
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
					auto& values = std::get<std::vector<std::string>>(value);
					for (const auto& elem : values)
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
					auto& values = std::get<std::vector<int>>(value);
					for (const auto& elem : values)
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
					auto& values = std::get<std::vector<double>>(value);
					for (const auto& elem : values)
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
					auto& values = std::get<std::vector<bool>>(value);
					for (const auto& elem : values)
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
	// end of struct json_t




	/**
	 * json 타입의 데이터를 사용하기 위한 클래스 .
	 */
	class Json
	{
	public:
		Json() = default;
		~Json() = default;
		std::vector<std::string> lines{};
		std::vector<json_obj_t> m_objects{};

		bool is_loaded{};
		//std::vector<json_t> m_datas{};

		inline bool file_copy(const char *pdest, const char *psour)
		{
			std::ifstream ifs(psour, std::ios::binary);
			if (!ifs.is_open())
			{
				// 원본 파일을 열 수 없는 경우 에러 처리
				return false;
			}

			std::ofstream ofs(pdest, std::ios::binary);
			if (!ofs.is_open())
			{
				// 복사할 파일을 생성할 수 없는 경우 에러 처리
				return false;
			}

			// 파일의 내용을 읽어와서 복사
			char buffer[4096];
			while (ifs.read(buffer, sizeof(buffer)))
			{
				ofs.write(buffer, sizeof(buffer));
			}

			// 나머지 남은 부분 복사
			ofs.write(buffer, ifs.gcount());

			return true;
		}
		inline bool loadFromFile(const std::string& filename)
		{
			std::ifstream inFile(filename);
			is_loaded = inFile.is_open();
			if (!is_loaded)
			{
				std::cerr << "No a file loaded " << std::endl;
				return false;
			}
			std::string line{};
			while (std::getline(inFile, line))
			{
				lines.emplace_back(line);
			}
			inFile.clear();
			inFile.seekg(0, std::ios::beg);
			std::string str((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
			is_loaded = parse(str);
			return is_loaded;
		}

		inline bool saveToFile(const std::string& file)
		{
			if (is_loaded == false)
			{
				std::cerr << "No a file loaded " << std::endl;
				return false;
			}

			// Get file name
			std::string filename = file; //std::filesystem::path(file).filename().string();
			size_t found = filename.rfind("."); // ".json" 제거 
			if (found != std::string::npos)
			{
				filename.erase(found);
			}
			file_copy(std::string{ filename + "_old.json" }.c_str(), file.c_str());

			std::ofstream ofs(file);
			if (!ofs.is_open())
				return false;

			for (const auto& line : lines) {
				ofs << line << std::endl; // 각 줄을 outFile에 쓰고, 줄 바꿈을 추가합니다.
			}
			ofs.close();

			return true;
		}



		inline bool replace_line(const json_t& src, size_t line_off)
		{
			if (is_loaded == false)
			{
				std::cerr << "No a file loaded " << std::endl;
				return false;
			}
			bool is_first = true;
			size_t idx{ line_off };

			auto& [index, value] = src;
			json_t::valueType elm_type{};
			json_t catch_data{};

			if (index == json_t::valueType::array)
			{
				if (json_t::is_obj_type(value))
				{
					auto& arr = std::get<json_obj_t>(value);

					for (auto& elm : arr)
					{
						while (idx < lines.size())
						{
							size_t pos = lines[idx].find(elm.first);
							if (pos != std::string::npos)
							{
								if (elm.second.type == json_t::valueType::array)
								{
									replace_line(elm.second, idx);
								}
								else
								{
									//catch_data.type = elm.second.type;
									//catch_data.value = elm.second.value;
									catch_data = elm.second;
									goto write_goto;
								}
								break;
							}
							idx++;
						}
						idx++;
					}
				}
				else
				{
					/*	auto varidx = json_t::get_varidx(idx);
						std::cout << std::to_string(idx) << std::endl;*/
					enum { _none, _jsonobj, _strings, _ints, _reals, _bools, _str, _int, _real, _bool };
					switch (value.index())
					{
					case _strings:
					{
						//"members": [\"tom\",\"wheel\",\"jason\"],
						char* start_addr = (char*)strchr(lines[idx].c_str(), '[');
						char* end_addr = (char*)strchr(lines[idx].c_str(), ']');
						if (start_addr == nullptr || end_addr == nullptr)
						{
							std::cout << "replace_line () not find keyword" << std::endl;
							return false;
						}
						size_t length = (end_addr - (start_addr));
						while (length)
						{
							if (*start_addr == '\"')
							{
								break;
							}
							++start_addr;
							--length;
						}
						std::string dest_str{ start_addr , length };
						auto& names = std::get< std::vector<std::string>>(value);
						std::string sour_str{ "\"" };
						for (const auto& elm : names)
						{
							sour_str.append(elm + std::string{ "\", \"" });
						}
						sour_str.erase(sour_str.end() - 3, sour_str.end()); //마지막 ", " 제거
						lines[idx].replace((lines[idx].find(dest_str)), dest_str.length(), sour_str);
					}
					break;
					case _ints:
					{
						//"numbers": [ 5, 4, 3, 2, 1 ],
						char* start_addr = (char*)strchr(lines[idx].c_str(), '[');
						char* end_addr = (char*)strchr(lines[idx].c_str(), ']');
						if (start_addr == nullptr || end_addr == nullptr)
						{
							std::cout << "replace_line () not find keyword" << std::endl;
							return false;
						}
						size_t length = (end_addr - (start_addr));
						while (length)
						{
							if (*start_addr == '-')
								break;

							if (*start_addr > 0x2f && *start_addr < 0x3a)
								break;

							++start_addr;
							--length;
						}

						std::string dest_str{ start_addr , length };
						auto& nums = std::get< std::vector<int>>(value);
						std::string sour_str{};
						for (const auto& elm : nums)
						{
							sour_str.append(std::to_string(elm) + ", ");
						}
						sour_str.erase(sour_str.end() - 2, sour_str.end()); //마지막 ", " 제거
						lines[idx].replace((lines[idx].find(dest_str)), dest_str.length(), sour_str);
					}
					break;
					case _reals:
					{
						//"members": [ 1.2, 2.456, 52.3456 ],
						char* start_addr = (char*)strchr(lines[idx].c_str(), '[');
						char* end_addr = (char*)strchr(lines[idx].c_str(), ']');
						if (start_addr == nullptr || end_addr == nullptr)
						{
							std::cout << "replace_line () not find keyword" << std::endl;
							return false;
						}
						size_t length = (end_addr - (start_addr));

						while (length)
						{
							if (*start_addr == '-')
								break;

							if (*start_addr > 0x2f && *start_addr < 0x3a)
								break;

							++start_addr;
							--length;
						}
						std::string dest_str{ start_addr , length };
						auto& reals = std::get< std::vector<double>>(value);
						std::string sour_str{};
						for (const auto& elm : reals)
						{
							sour_str.append(std::to_string(elm) + ", ");
						}
						sour_str.erase(sour_str.end() - 2, sour_str.end()); //마지막 ", " 제거
						lines[idx].replace((lines[idx].find(dest_str)), dest_str.length(), sour_str);
					}
					break;
					case _bools:
					{
						//"checks": [ false, true, true, true, false ],
						char* start_addr = (char*)strchr(lines[idx].c_str(), '[');
						char* end_addr = (char*)strchr(lines[idx].c_str(), ']');
						if (start_addr == nullptr || end_addr == nullptr)
						{
							std::cout << "replace_line () not find keyword" << std::endl;
							return false;
						}
						size_t length = (end_addr - (start_addr));
						while (length)
						{
							if (*start_addr == 'f'
								|| *start_addr == 'F'
								|| *start_addr == 't'
								|| *start_addr == 'T')
							{
								break;
							}
							++start_addr;
							--length;
						}

						std::string dest_str{ start_addr, length };
						auto& btns = std::get< std::vector<bool>>(value);
						std::string sour_str{};
						for (const auto& elm : btns)
						{
							sour_str.append((elm ? "true" : "false") + std::string{ ", " });
						}
						sour_str.erase(sour_str.end() - 2, sour_str.end()); //마지막 ", " 제거
						lines[idx].replace((lines[idx].find(dest_str)), dest_str.length(), sour_str);
					}
					break;
					default:
						std::cout << "replace_line () No matching case found for value.index" << std::endl;
						return false;
					}

				}
			}


		write_goto:

			char* start_addr = (char*)strchr(lines[idx].c_str(), ':');
			char* end_addr = (char*)strchr(lines[idx].c_str(), '}');
			if (start_addr == nullptr || end_addr == nullptr)
			{
				std::cout << "replace_line () not find keyword" << std::endl;
				return false;
			}
			size_t length = (end_addr - start_addr) - 1;
			std::string dest_str{ start_addr + 1, length };
			trim(dest_str);
			switch (catch_data.type)
			{
			case json_t::valueType::string:
			case json_t::valueType::real:
			case json_t::valueType::integer:
			case json_t::valueType::boolean:
			{
				std::size_t pos = lines[idx].find(dest_str);
				if (pos != std::string::npos)
				{
					lines[idx].replace(pos, dest_str.length(), catch_data.toJsonString());
				}
			}
			break;

			case json_t::valueType::none:
			default:
				break;

			}
			//end of switch

			return false;
		}

		inline bool replace_value(const json_obj_t& src_data)
		{
			if (is_loaded == false)
			{
				std::cerr << "No a file loaded " << std::endl;
				return false;
			}

			for (auto& jdata : m_objects)
			{
				for (const auto& pair : src_data)
				{
					auto it = jdata.find(pair.first);
					if (it != jdata.end())
					{
						{
							// change object_data
							it->second = pair.second;
						}
						{
							// change lines data
							size_t idx{ };
							//for (auto& elm : lines)
							for (idx; idx < lines.size(); ++idx)
							{
								if (lines[idx].find(it->first) != std::string::npos)
								{
									json_t value = it->second;
									switch (value.type)
									{
									case json_t::valueType::object:
									{
										replace_line(it->second, idx + 1); //jdata,
									}
									break;
									case json_t::valueType::array:
									{
										replace_line(it->second, idx); //jdata,
									}
									break;
									case json_t::valueType::string:
									{
										std::size_t pos = lines[idx].find(pair.first);
										pos = lines[idx].find(":", (pos + pair.first.length()));
										if (pos == std::string::npos)
										{
											std::cout << "replace_value() not find keyword" << std::endl;
											return false;
										}
										char* start_addr = (char*)strchr(lines[idx].c_str(), ':');
										char* end_addr = (char*)strchr(lines[idx].c_str(), ',');
										if (start_addr == nullptr || end_addr == nullptr)
										{
											std::cout << "replace_value () not find keyword" << std::endl;
											return false;
										}
										size_t length = (end_addr - (start_addr));
										while (length)
										{
											if (*start_addr == '\"')
											{
												break;
											}
											++start_addr;
											--length;
											++pos;
										}

										std::string dest_str{ start_addr, length };
										std::string sour_str{ "\"" + std::get<std::string>(pair.second.value) + "\"" };
										lines[idx].replace((pos), dest_str.length(), sour_str);
									}
									break;

									case json_t::valueType::integer:
									{
										std::size_t pos = lines[idx].find(pair.first);
										pos = lines[idx].find(":", (pos + pair.first.length()));
										if (pos == std::string::npos)
										{
											std::cout << "replace_value() not find keyword" << std::endl;
											return false;
										}
										char* start_addr = (char*)strchr(lines[idx].c_str(), ':');
										char* end_addr = (char*)strchr(lines[idx].c_str(), ',');
										if (start_addr == nullptr || end_addr == nullptr)
										{
											std::cout << "replace_value () not find keyword" << std::endl;
											return false;
										}
										size_t length = (end_addr - (start_addr));
										while (length)
										{
											if (*start_addr == '-')
												break;

											if (*start_addr > 0x2f && *start_addr < 0x3a)
												break;

											++start_addr;
											--length;
											++pos;
										}
										std::string dest_str{ start_addr, length };
										std::string sour_str{ std::to_string(std::get<int>(pair.second.value)) };
										lines[idx].replace((pos), dest_str.length(), sour_str);

									}
									break;

									case json_t::valueType::real:
									{
										std::size_t pos = lines[idx].find(pair.first);
										pos = lines[idx].find(":", (pos + pair.first.length()));
										if (pos == std::string::npos)
										{
											std::cout << "replace_value() not find keyword" << std::endl;
											return false;
										}
										//lines[idx].replace(pos, .length(), catch_data.toJsonString());
										char* start_addr = (char*)strchr(lines[idx].c_str(), ':');
										char* end_addr = (char*)strchr(lines[idx].c_str(), ',');
										if (start_addr == nullptr || end_addr == nullptr)
										{
											std::cout << "replace_value () not find keyword" << std::endl;
											return false;
										}
										size_t length = (end_addr - (start_addr));
										while (length)
										{
											if (*start_addr == '-')
												break;

											if (*start_addr > 0x2f && *start_addr < 0x3a)
												break;

											++start_addr;
											--length;
											++pos;
										}
										std::string dest_str{ start_addr, length };
										std::string sour_str{ std::to_string(std::get<double>(pair.second.value)) };
										lines[idx].replace((pos), dest_str.length(), sour_str);
									}
									break;

									case json_t::valueType::boolean:
									{
										std::size_t pos = lines[idx].find(pair.first);
										pos = lines[idx].find(":", (pos + pair.first.length()));
										if (pos == std::string::npos)
										{
											std::cout << "replace_value() not find keyword" << std::endl;
											return false;
										}
										//lines[idx].replace(pos, .length(), catch_data.toJsonString());
										char* start_addr = (char*)strchr(lines[idx].c_str(), ':');
										char* end_addr = (char*)strchr(lines[idx].c_str(), ',');
										if (start_addr == nullptr || end_addr == nullptr)
										{
											std::cout << "replace_value () not find keyword" << std::endl;
											return false;
										}										
										size_t length = (end_addr - (start_addr));
										while (length)
										{
											if (*start_addr == 'f'
												|| *start_addr == 'F'
												|| *start_addr == 't'
												|| *start_addr == 'T')
											{
												break;
											}
											++start_addr;
											--length;
											++pos;
										}

										std::string dest_str{ start_addr, length };
										std::string sour_str{ std::get<bool>(pair.second.value) ? "true" : "false" };
										lines[idx].replace((pos), dest_str.length(), sour_str);
									}
									break;
									case json_t::valueType::none:
									default:
										break;

									}
									//end of switch

									//if (elm.find(it->first) != std::string::npos)
									//{
									//}
								}
							}
						}
						return true;
					}
				}
			}
			return false;
		}


		inline std::string toJsonString() const
		{
			if (is_loaded == false)
			{
				std::cerr << "No a file loaded " << std::endl;
				return  std::string{};
			}
			std::stringstream ss;

			ss << "{ \n";
			using jvtype = json_t::valueType;

			for (const auto& data : this->m_objects)
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
						auto& obj = std::get<json_obj_t>(value.value);
						bool first = true;
						for (const auto& [key, value] : obj)
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
							auto& arr = std::get<json_obj_t>(value.value);
							bool first = true;
							for (const auto& elem : arr)
							{
								if (elem.second.type == json_t::valueType::array)
								{
									if (!first)
										ss << ",\n";//ss << ", \n    {";//\n

									ss << "  {\n  ";

									ss << "\"" << elem.first << "\" : ";
									ss << elem.second.toJsonString() << "\n  }";
								}
								else
								{
									if (!first)
										ss << ", \n";//\n

									ss << "    {";

									ss << "\"" << elem.first << "\" : ";
									ss << elem.second.toJsonString() << "}";
								}
								first = false;
							}
						}
						break;
						case arry_type::string:
						{
							bool first = true;
							auto& values = std::get<std::vector<std::string>>(value.value);
							for (const auto& elem : values)
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
							auto& values = std::get<std::vector<int>>(value.value);
							for (const auto& elem : values)
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
							auto& values = std::get<std::vector<double>>(value.value);
							for (const auto& elem : values)
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
							auto& values = std::get<std::vector<bool>>(value.value);
							for (const auto& elem : values)
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

					for (const auto& [key, value] : data)
					{
						ss << "\"" << key << "\":" << value.toJsonString();
					}
				}
				ss << "\n";
			}

			ss << "\n } \n";
			return ss.str();
		}

		inline void trim(std::string& str)
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

		inline const char* find_first_char(const std::string& str, char ch)
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

		inline const char* find_balance_char(const std::string& str, char open_key = '{', char close_key = '}')
		{
			size_t open_pos = str.find(open_key);
			if (open_pos == std::string::npos)
				return &str[0]; // 없는 경우


			size_t close_pos = 0;
			int balance = 1; // open_key 를 만나면 1 증가, close_key 를 만나면 1 감소
			size_t idx = open_pos + 1;
			while (idx < str.size() && balance)
			{
				if (str[idx % str.size()] == open_key)
				{
					balance++;
				}
				else if (str[idx % str.size()] == close_key)
				{
					balance--;
				}
				idx++;
			}
			return &str[idx % str.size()];

		}

		inline const char* find_last_char(const std::string& str, char ch)
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

		inline int replace(std::string& str, const char* before, const char* after)
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

		inline int replace_trim(std::string& str, const char* before, const char* after)
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
			constexpr std::string_view whitespace{ " \t\n\r\f\v" };

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

		inline bool isInteger(const std::string_view& str)
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
		inline bool isBool(const std::string_view& str)
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

		inline bool isReal(const std::string_view& str)
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

		inline bool parseObject(std::string_view str, json_t& data, std::size_t& ret_idx)
		{
			std::size_t i{};

			auto get_number = [&str, &i](bool& is_double) -> std::string
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
					size_t head_pos = i;

					std::string ret{};
					char open_char{};
					switch (find_c)
					{
					case '}':	open_char = '{';		break;
					case ']':	open_char = '[';	  break;
					default:
					{
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
					}
					break;
					}
					int balance = 1;
					while (i < str.size())
					{
						if (str[i % str.size()] == open_char)
						{
							balance++;
						}
						else if (str[i % str.size()] == find_c)
						{
							balance--;
						}

						if (balance == 0)
						{
							ret = str.substr(head_pos, (i - head_pos));
							return ret;
						}

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

					/*      while (i < str.size())
								{
									std::string key = get_key();
									if (key.empty())
										break;
									json_t ret_data{};

								}*/
								// end of while
					std::string arg_str{ str };
					char* start_addr = (char*)find_first_char(arg_str, '{');
					char* end_addr = (char*)find_balance_char(arg_str, '{', '}');
					size_t start_idx = start_addr - (char*)arg_str.c_str();
					size_t length = end_addr - start_addr;
					std::string obj_str = arg_str.substr(start_idx, length);

					json_obj_t obj{};
					size_t inner_idx = 0;
					while (inner_idx < obj_str.size())//while (i < str.size())
					{
						if ((obj_str[(inner_idx)] == ' ' || obj_str[(inner_idx)] == '}') == false)
						{
							std::string key = get_key();
							if (key.empty())
								break;
							json_t ret_data{};
							size_t pos = obj_str.find(':') + 1;
							inner_idx = inner_idx + pos;
							if (parseObject(&obj_str[(inner_idx) % obj_str.size()], ret_data, inner_idx))
							{
								obj.emplace(key, ret_data);
							}

						}
						inner_idx++;
						i = inner_idx;
					}
					// while
					inner_idx++;
					ret_idx = ret_idx + inner_idx;
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
						data = std::stod((char*)cut_str.c_str());
					}
					else
					{
						data = std::stoi((char*)cut_str.c_str());
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

		inline int split_balance(char* arg_str, char** argv, char open_key = '{', char close_key = '}', size_t max = 1024)
		{
			int argc = 0;
			char* tok = nullptr;
			char* next_ptr = nullptr;
			size_t str_len{};
			std::string str{ arg_str };
			size_t arg_size = str.size();
			while (str_len < arg_size)
			{
				char* start_addr = (char*)find_first_char(str, open_key);
				const char* end_addr = find_balance_char(str, open_key, close_key);
				size_t length = end_addr - start_addr;
				size_t start_idx = std::string{ str.c_str() }.find(open_key);
				if (start_idx != std::string::npos)
				{
					std::string obj_str = str.substr(start_idx, length);
					if (obj_str.empty() == false)
					{
						str.erase(0, start_idx + length);
						//str_len = str_len + length;    
						argv[argc] = &arg_str[str_len];
						argc++;
					}
					else
					{
						break;
					}
					str_len = str_len + start_idx + length;

				}

				str_len++;
			}



			/* for (tok = strtok_s(arg_str, delim_chars, &next_ptr); tok; tok = strtok_s(NULL, delim_chars, &next_ptr))
			 {
				 argv[argc++] = tok;
				 if (argc >= max)
					 break;
			 }*/
			return argc;
		}

		inline int split(char* arg_str, char** argv, const char* delim_chars, size_t max = 1024)
		{
			int argc = 0;
			char* tok = nullptr;
			char* next_ptr = nullptr;

			for (tok = strtok_s(arg_str, delim_chars, &next_ptr); tok; tok = strtok_s(NULL, delim_chars, &next_ptr))
			{
				argv[argc++] = tok;
				if (argc >= max)
					break;
			}
			return argc;
		}
		inline bool parseArray(std::string_view array_str, json_t& ret_data /*, std::size_t& ret_idx*/)
		{
			if (array_str.empty())
				return false;

			auto get_key = [](const std::string& str, std::size_t& idx) -> std::string
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
			char* argv[elem_max] = {};
			bool is_object = (array_str.find('{') != std::string::npos && array_str.find('}') != std::string::npos);



			if (is_object)
			{
				int argc = split_balance((char*)array_str.data(), argv);
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
				int argc = split((char*)array_str.data(), argv, delim_chars, elem_max);
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

		inline bool parse(const std::string& arrayString)
		{

			auto get_str = [&](auto& str) -> std::string
				{
					char* start_addr = strchr(str, '{');
					char* end_addr = strchr(str, '}');
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

			char* start_addr = (char*)find_first_char(arrayString, '{');
			char* end_addr = (char*)find_last_char(arrayString, '}');
			size_t start_idx = start_addr - (char*)arrayString.c_str();
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
					break;

				// key 문자에서 앞뒤로 " 제거
				std::string key(obj_str.data() + pos, end_key - pos);
				pos = +(replace_trim(key, "\"", ""));
				if (key.empty())
					break;;
				

				pos = end_key + 1;
				json_t ret_object{};
				std::string_view view_str(&obj_str[pos]);
				if (parseObject(&obj_str[pos], ret_object, pos))
				{
					json_obj_t obj{};
					obj.emplace(key, ret_object);
					m_objects.emplace_back(obj);
					//m_datas.emplace_back(ret_object);
				}
				else
				{
					break;
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