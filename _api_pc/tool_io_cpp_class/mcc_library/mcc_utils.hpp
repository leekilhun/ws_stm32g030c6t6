#pragma once
#ifndef _COMMON_MCUUTILS_HPP
#define _COMMON_MCUUTILS_HPP

/*

lee kil hun
gns.lee2@samsung.com
2023.04

*/

#include <array>
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <iomanip>
#include <algorithm>
#include <chrono>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ratio>
#include <random>
#include <sstream>
#include <numeric>
#include <execution>
#include <ctime>

#include <locale>
#include <codecvt>
#include <cstdlib>
#include <cwchar>

#include <mutex>
#include <string_view>
#include <cstdarg>

#include <windows.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif


#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS  0
#endif


#ifndef GMAX
#define GMAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef GMIN
#define GMIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef GMAP
#define GMAP(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

#ifndef GCONSTRAIN
#define GCONSTRAIN(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif




namespace crc
{
	const uint16_t MODBUS_CRC16Table[] =
	{ 0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241, 0XC601, 0X06C0, 0X0780,
	   0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440, 0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1,
	   0XCE81, 0X0E40, 0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841, 0XD801,
	   0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40, 0X1E00, 0XDEC1, 0XDF81, 0X1F40,
	   0XDD01, 0X1DC0, 0X1C80, 0XDC41, 0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680,
	   0XD641, 0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040, 0XF001, 0X30C0,
	   0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240, 0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501,
	   0X35C0, 0X3480, 0XF441, 0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
	   0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840, 0X2800, 0XE8C1, 0XE981,
	   0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41, 0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1,
	   0XEC81, 0X2C40, 0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640, 0X2200,
	   0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041, 0XA001, 0X60C0, 0X6180, 0XA141,
	   0X6300, 0XA3C1, 0XA281, 0X6240, 0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480,
	   0XA441, 0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41, 0XAA01, 0X6AC0,
	   0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840, 0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01,
	   0X7BC0, 0X7A80, 0XBA41, 0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
	   0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640, 0X7200, 0XB2C1, 0XB381,
	   0X7340, 0XB101, 0X71C0, 0X7080, 0XB041, 0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0,
	   0X5280, 0X9241, 0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440, 0X9C01,
	   0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40, 0X5A00, 0X9AC1, 0X9B81, 0X5B40,
	   0X9901, 0X59C0, 0X5880, 0X9841, 0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81,
	   0X4A40, 0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41, 0X4400, 0X84C1,
	   0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641, 0X8201, 0X42C0, 0X4380, 0X8341, 0X4100,
	   0X81C1, 0X8081, 0X4040
	};



	inline int crc16_modbus_cal(uint8_t* p_data, uint16_t length) {
		uint8_t nTemp;

		uint16_t ret_crc = 0xFFFF;

		while (length--)
		{
			nTemp = *p_data++ ^ ret_crc;
			ret_crc >>= 8;
			ret_crc ^= MODBUS_CRC16Table[nTemp];
		}

		return ret_crc;
	}


	inline void crc16_modbus_update(uint16_t* crc_in, uint8_t data) {
		constexpr uint16_t poly = 0xA001;
		uint16_t crc = *crc_in;
		uint8_t i;
		/* Exclusive-OR the byte with the CRC */
		crc ^= data; //*(pDataBuffer + iByte);

		/* Loop through all 8 data bits */
		for (i = 0; i <= 7; i++) {
			/* If the LSB is 1, shift the CRC and XOR the polynomial mask with the CRC */
			// Note - the bit test is performed before the rotation, so can't move the << here
			if (crc & 0x0001) {
				crc >>= 1;
				crc ^= poly;
			}
			else {
				// Just rotate it
				crc >>= 1;
			}
		}
		*crc_in = crc;
	}

	inline void crc16_update(uint16_t* crc_in, uint8_t data) {
		constexpr uint16_t poly = 0xA001;
		uint16_t crc = *crc_in;
		uint8_t i;
		/* Exclusive-OR the byte with the CRC */
		crc ^= data; //*(pDataBuffer + iByte);

		/* Loop through all 8 data bits */
		for (i = 0; i <= 7; i++) {
			/* If the LSB is 1, shift the CRC and XOR the polynomial mask with the CRC */
			// Note - the bit test is performed before the rotation, so can't move the << here
			if (crc & 0x0001) {
				crc >>= 1;
				crc ^= poly;
			}
			else {
				// Just rotate it
				crc >>= 1;
			}
		}
		*crc_in = crc;
	}

}
// end of namespace crc

namespace TinyC
{
	template<typename T>
	class Que {
	public:
		explicit Que(std::size_t length) : m_Length(length), m_Head(0), m_Tail(0), m_Buff(length) {}

		inline int Available() {
			return static_cast<int>((m_Length + (m_Head - m_Tail)) % m_Length);
		}

		inline bool Put(T data) {
			bool ret = true;
			uint32_t next_in;
			next_in = (m_Head + 1) % m_Length;
			if (next_in != m_Tail) {
				m_Buff[m_Head % m_Length] = data;
				m_Head = next_in;
			}
			else {
				ret = false;
			}
			return ret;
		}

		inline bool Get(T* p_data) {
			bool ret = true;
			*(p_data) = m_Buff[m_Tail];
			if (m_Tail != m_Head) {
				m_Tail = (m_Tail + 1) % m_Length;
			}
			else {
				ret = false;
			}
			return ret;
		}

		inline T Pop(uint32_t addr) {
			if (addr > (m_Length - 1)) {
				return m_Buff[m_Tail];
			}
			return m_Buff[addr];
		}

		inline void Pop(uint32_t addr, T* p_data) {
			if (addr > (m_Length - 1)) {
				return;
			}
			*(p_data) = m_Buff[addr];
		}

		inline uint32_t GetSize() {
			return static_cast<uint32_t>((m_Head - m_Tail) % m_Length);
		}

		inline void Flush() {
			m_Head = m_Tail = 0;
		}

		inline T operator[](uint32_t addr) {
			return Pop(addr);
		}

	private:
		std::size_t m_Length;
		uint32_t m_Head;
		uint32_t m_Tail;
		std::vector<T> m_Buff;
	};
}

namespace tim
{


	using nano_t = std::nano;
	using micro_t = std::micro;
	using milli_t = std::milli;
	using second_t = std::ratio<1>;
	using minute_t = std::ratio<60>;
	using hour_t = std::ratio<3600>;

	template <typename TimeUnit>
	using duration_t = std::chrono::duration<double, TimeUnit>;

	using nanoseconds_t = duration_t<nano_t>;
	using microseconds_t = duration_t<micro_t>;
	using milliseconds_t = duration_t<milli_t>;
	using seconds_t = duration_t<second_t>;
	using minutes_t = duration_t<minute_t>;
	using hours_t = duration_t<hour_t>;

	using high_resolution_clock_t = std::chrono::high_resolution_clock;
	using time_point_t = std::chrono::time_point<high_resolution_clock_t>;

	inline auto& now = high_resolution_clock_t::now;  //


	inline unsigned int seed()
	{
		return static_cast<unsigned int>(high_resolution_clock_t::now().time_since_epoch().count());
	}

	// used to initialize random engine
	inline unsigned int get_current_tick()
	{
		using std::chrono::duration;
		using std::chrono::duration_cast;
		auto current =
			duration_cast<std::chrono::milliseconds>(now().time_since_epoch());

		return static_cast<unsigned int>(current.count());
	}

	// to convert time (in TimeUnit) to type double
	template <typename TimeUnit>
	double time_difference_in_unit(
		const time_point_t& start_time,
		const time_point_t& end_time)
	{
		using std::chrono::duration;
		using std::chrono::duration_cast;

		if constexpr (std::is_same_v<TimeUnit, second_t>)
		{
			auto period_of_time =
				duration_cast<std::chrono::duration<double, second_t>>(end_time - start_time);
			return period_of_time.count();
		}
		else
		{
			auto period_of_time =
				duration_cast<std::chrono::duration<double, TimeUnit>>(end_time - start_time);

			return period_of_time.count();
		}
	}

	class stop_watch
	{
	private:
		mutable high_resolution_clock_t m_clock;
		mutable time_point_t m_start_time;
		/*auto now() {
			return high_resolution_clock_t::now();
		}*/
	public:
		stop_watch() : m_start_time{ now() }
		{ }

		void reset() const { this->m_start_time = now(); }

		template<typename TimeUnit = milli_t>
		auto elapsed(bool bReset = true) const
		{
			auto rlt = time_difference_in_unit<TimeUnit>(this->m_start_time, now());
			if (bReset) reset();
			return rlt;
		}

		template<typename TimeUnit = milli_t>
		std::string elapsed_time(bool bReset = true, TimeUnit dummy_time = TimeUnit{}) const
		{
			std::ostringstream os;

			os << time_difference_in_unit<TimeUnit>(this->m_start_time, now());

			if constexpr (std::is_same_v<TimeUnit, nano_t>)
				os << " nano-seconds";
			else if constexpr (std::is_same_v<TimeUnit, micro_t>)
				os << " micro-seconds";
			else if constexpr (std::is_same_v<TimeUnit, milli_t>)
				os << " milli-seconds";
			else if constexpr (std::is_same_v<TimeUnit, second_t>)
				os << " seconds";
			else if constexpr (std::is_same_v<TimeUnit, minute_t>)
				os << " minutes";
			else if constexpr (std::is_same_v<TimeUnit, hour_t>)
				os << " hours";

			if (bReset) reset();
			return os.str();
		}
	};
	// end of class stop_watch


	//[2022-10-04 13:45:51'11]
	inline void NowStr(char* p_str, int max_len)
	{
		auto now = std::chrono::system_clock::now();
		auto t = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		tm local_time;
		localtime_s(&local_time, &t);

		std::strftime(p_str, max_len, "[%Y-%m-%d %H:%M:%S'", &local_time);
		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(3) << ms.count() << "] " /*\n*/;

		size_t len = std::strlen(p_str);
		strncat_s(p_str + len, max_len - len, oss.str().c_str(), _TRUNCATE);
	}

	inline std::string DateStr()
	{
		auto now = std::chrono::system_clock::now();
		auto t = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		char buffer[100];
		struct tm timeinfo;
		localtime_s(&timeinfo, &t);
		std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d]", &timeinfo);
		return std::string(buffer);
	}

	inline std::string TimeStr()
	{
		auto now = std::chrono::system_clock::now();
		auto t = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		char buffer[100];
		struct tm timeinfo;
		localtime_s(&timeinfo, &t);
		std::strftime(buffer, sizeof(buffer), "[%H:%M:%S'", &timeinfo);
		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(3) << ms.count() << "] " /*\n*/;
		return std::string(buffer) + oss.str();
	}

	inline std::string NowStr()
	{

		auto now = std::chrono::system_clock::now();
		auto t = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		char buffer[100];
		struct tm timeinfo;
		localtime_s(&timeinfo, &t);
		std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S'", &timeinfo);
		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(3) << ms.count() << "] " /*\n*/;
		return std::string(buffer) + oss.str();
	}


	inline unsigned int millis(void) {
		//return 0;
		using std::chrono::duration;
		using std::chrono::duration_cast;
		auto current =
			duration_cast<std::chrono::milliseconds>(now().time_since_epoch());

		return static_cast<unsigned int>(current.count());
	}

	inline unsigned int micros(void) {
		using std::chrono::duration;
		using std::chrono::duration_cast;
		auto current =
			duration_cast<std::chrono::microseconds>(now().time_since_epoch());

		return static_cast<unsigned int>(current.count());
	}

	inline void delay(unsigned int ms) {
		unsigned int pre_time;

		pre_time = millis();
		while (1) {
			if (millis() - pre_time >= ms)
				break;
		}
	}
}
//end of namaspace tim 



namespace file
{

	enum FILE_ERR_CODE
	{
		FILE_ERR_MAKE_DEL = -1,
		FILE_ERR_SUCCESS = 0,
		FILE_ERR_NO_DIFFERENCE,
		FILE_ERR_READ_FILE,
		FILE_ERR_MAKE_COPYFILE,
		FILE_ERR_WRITE_COPYFILE
	};

	namespace fs = std::filesystem;
	inline void CurrDir(char* p_buff, int len)
	{
		fs::path curr_path = fs::current_path();
		std::string curr_path_str = curr_path.string();
		strncpy_s(p_buff, len, curr_path_str.c_str(), curr_path_str.size() + 1);//strncpy(p_buff, curr_path_str.c_str(), len - 1);
		p_buff[len - 1] = '\0';
	}
	inline void CurrDir(std::string& ret_str)
	{
		fs::path curr_path = fs::current_path();
		ret_str = curr_path.string();
		//strncpy_s(p_buff, len, curr_path_str.c_str(), curr_path_str.size() + 1);//strncpy(p_buff, curr_path_str.c_str(), len - 1);
		//p_buff[len - 1] = '\0';
	}

	inline int MakeDir(const char* dir_name)
	{
		if (fs::create_directory(dir_name)) {
			return FILE_ERR_SUCCESS; //
		}
		else {
			return FILE_ERR_MAKE_DEL; // 
		}
	}




	inline int DeleteDir(const char* dir_name)
	{
		if (fs::remove_all(dir_name)) {
			return FILE_ERR_SUCCESS; // 
		}
		else {
			return FILE_ERR_MAKE_DEL; // 
		}
	}

	inline bool isFile(const fs::directory_entry& entry)
	{
		return fs::is_regular_file(entry);
	}

	inline bool isDir(const fs::directory_entry& entry)
	{
		return fs::is_directory(entry);
	}

	inline bool isDirExists(const char* dir)
	{
		return fs::exists(dir) && fs::is_directory(dir);
	}

	inline bool isFileExists(const char* file)
	{
		return fs::exists(file) && fs::is_regular_file(file);
	}




	inline int SearchDir(const char* dir_str)
	{
		std::string dir_name = dir_str;
		std::string path = dir_name + "/*";
		int ret = FILE_ERR_SUCCESS;
		std::vector<std::filesystem::directory_entry> entries;

		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_directory() && entry.path().filename() != "." && entry.path().filename() != "..") {
				ret += SearchDir((const char*)entry.path().c_str());  // 
			}
			else if (entry.is_regular_file() && entry.path().filename().string()[0] != '.') {
				entries.push_back(entry);
			}
		}
		ret += static_cast<int>(entries.size());
		return ret;
	}

	inline int GetFileCount(const char* dir_str, bool inc_dir)
	{
		fs::path dir_path(dir_str);
		int ret = FILE_ERR_SUCCESS;
		for (const auto& entry : fs::directory_iterator(dir_path)) {
			if (entry.is_directory() && inc_dir) {
				ret += GetFileCount((const char*)entry.path().c_str(), inc_dir);
			}
			else if (entry.is_regular_file()) {
				ret++;
			}
		}
		return ret;
	}


	inline bool isHidden(const std::filesystem::path& path) {
		return path.filename().string().front() == '.';
	}


	inline bool isLeafDir(const char* dir_str)
	{
		std::filesystem::path dir_path(dir_str);
		std::filesystem::directory_iterator end_iter;
		bool ret = true;
		for (const auto& dir_itr : std::filesystem::directory_iterator(dir_path)) {
			if (dir_itr.is_directory() && !isHidden(dir_itr.path())) {
				ret = false;
				break;
			}
		}

		return ret;
	}


	inline int DeleteFilesInDir(const char* dir_str, bool inc_dir)
	{
		fs::path dir_path(dir_str);
		int ret = FILE_ERR_SUCCESS;

		if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
			std::cout << "Invalid directory path!" << std::endl;
			return ret;
		}

		for (const auto& entry : fs::directory_iterator(dir_path)) {
			const auto& path = entry.path();

			if (fs::is_directory(path) && !fs::is_empty(path)) {
				if (inc_dir) {
					ret += DeleteFilesInDir(path.string().c_str(), inc_dir); // 
				}
			}
			else if (fs::is_regular_file(path)) {
				fs::remove(path);
				ret++;
			}
		}

		if (inc_dir) {
			fs::remove_all(dir_path);
		}

		return ret;
	}

	inline bool FileCopy(const char* pdest, const char* psour)
	{
		std::ifstream ifs(psour, std::ios::binary);
		if (!ifs.is_open()) {
			// 원본 파일을 열 수 없는 경우 에러 처리
			return false;
		}

		std::ofstream ofs(pdest, std::ios::binary);
		if (!ofs.is_open()) {
			// 복사할 파일을 생성할 수 없는 경우 에러 처리
			return false;
		}

		// 파일의 내용을 읽어와서 복사
		char buffer[4096];
		while (ifs.read(buffer, sizeof(buffer))) {
			ofs.write(buffer, sizeof(buffer));
		}

		// 나머지 남은 부분 복사
		ofs.write(buffer, ifs.gcount());

		return true;
	}

	inline std::error_code CopyFilesInDir_binary(const char* dst_dir, const char* src_dir, bool over_write, bool inc_dir)
	{
		std::filesystem::path src_path{ src_dir };
		src_path /= "*.*";
		std::error_code ec;
		std::filesystem::directory_iterator dir_iter{ src_path, ec };

		if (ec || dir_iter == std::filesystem::end(dir_iter)) {
			return std::make_error_code(std::errc::no_such_file_or_directory);
		}

		std::filesystem::path dst_path{ dst_dir };

		for (const auto& entry : dir_iter) {
			if (entry.is_directory()) {
				if (inc_dir) {
					std::error_code sub_dir_err = CopyFilesInDir_binary(
						(const char*)(dst_path / entry.path().filename()).c_str(),
						(const char*)entry.path().c_str(),
						over_write,
						inc_dir);
					if (sub_dir_err)
						return sub_dir_err;
				}
				continue;
			}

			std::ifstream src_file{ entry.path().c_str(), std::ios::binary };
			if (!src_file) {
				return std::make_error_code(std::errc::no_such_file_or_directory);
			}

			std::vector<char> buff(std::filesystem::file_size(entry));
			src_file.read(buff.data(), buff.size());

			std::filesystem::path dst_file_path{ dst_path / entry.path().filename() };
			if (!isDirExists((const char*)dst_path.c_str())) {
				MakeDir((const char*)dst_path.c_str());
			}

			if (isFileExists((const char*)dst_file_path.c_str())) {
				if (over_write) {
					std::ofstream dst_file{ dst_file_path.c_str(), std::ios::binary };
					if (dst_file) {
						dst_file.write(buff.data(), buff.size());
					}
					else {
						return std::make_error_code(std::errc::permission_denied);
					}
				}
			}
			else {
				std::ofstream dst_file{ dst_file_path.c_str(), std::ios::binary };
				if (dst_file) {
					dst_file.write(buff.data(), buff.size());
				}
				else {
					return std::make_error_code(std::errc::permission_denied);
				}
			}
		}

		return std::error_code{};
	}

inline int GetFileSize(const char* file_path)
{
	std::ifstream ifs(file_path, std::ios::binary);
	if (!ifs.is_open()) {
		// 원본 파일을 열 수 없는 경우 에러 처리
		std::cout<< "file open error" << std::endl;
		return 0;
	}

	ifs.seekg(0, std::ios::end);
	int size = (int)ifs.tellg();
	ifs.close();

	return size;
}


}
//end of file namespace

namespace step
{
	template <typename StepUnit = uint16_t>
	struct state_st
	{
		StepUnit curr_step{};
		StepUnit pre_step{};
		StepUnit wait_step{};
		uint32_t prev_ms{};
		uint32_t elap_ms{};
		StepUnit retry_cnt{};
		TinyC::Que<StepUnit> stepQue{};
		bool wait_resp{};//true - wait step complete, false - completed step

		state_st() : curr_step{}, pre_step{}, wait_step{}, prev_ms{}, elap_ms{}, retry_cnt{}, stepQue{256}, wait_resp{}
		{}

		inline void SetStep(StepUnit step) {
			elap_ms = tim::millis() - prev_ms;
			prev_ms = tim::millis();
			pre_step = curr_step;
			curr_step = step;
		}

		inline StepUnit GetStep() const {
			return curr_step;
		}

		inline bool LessThan(uint32_t msec) {
			elap_ms = tim::millis() - prev_ms;
			if (elap_ms < msec)
				return true;
			else
				return false;
		}

		inline bool MoreThan(uint32_t msec) {
			return !LessThan(msec);
		}

		inline bool AvailableStep() const {
			return !wait_resp;
		}


		inline bool IsInStep(StepUnit step) {
			if (stepQue.m_Head)
			{
				if (stepQue.m_Buff[stepQue.m_Head - 1] != step
					|| stepQue.m_Buff[stepQue.m_Head - 1] != step + 1
					|| stepQue.m_Buff[stepQue.m_Head - 1] != step + 2)
				{
					return true;
				}
			}
			return false;
		}

	};

}
// end of namespace step


#endif
//_COMMON_MCUUTILS_HPP