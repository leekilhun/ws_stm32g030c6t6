#pragma once
#ifndef _MCC_CONVERSION_HPP
#define _MCC_CONVERSION_HPP


#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <string>
#include <iostream>
#include <clocale>
#include <cstring>
#include <type_traits>
#include <windows.h>



namespace mcc
{

    namespace conversion
    {

        /**
         * @brief Converts from std::wstring to std::string with codepage
         * 
         * If fails, returns empty string, std::string("")
         * 
         * @param wstr for std::wstring to convert
         * @param codepage for Code Page, default = CP_UTF8
         * @return std::string for the converted string
         * 
         * @sa <a target ="_blank" href="002-conversion_8cpp_source.html">002-conversion.cpp</a>
         * 
         */
        inline std::string wstring_to_string(const std::wstring& wstr, 
            unsigned int codepage = CP_UTF8)
        {
            if (wstr.empty()) return "";

            // https://goo.gl/acoQBt
            int str_len = WideCharToMultiByte(
                codepage,
                0, // dwFlag
                wstr.c_str(),
                (int)wstr.size(), NULL, 0, NULL, NULL);

            // if failed to compute the byte count of the string
            if (str_len == 0) return "";

            // prepare a string buffer to
            // hold the converted string
            // do not +1 to str_len,
            // because std::string manages terminating null
            std::string str(str_len, '\0');

            int converted = WideCharToMultiByte(
                codepage, 0, wstr.c_str(), (int)wstr.size(),
                &str[0], str_len, NULL, NULL);

            return (converted == 0 ? "": str);
        }

        /** @example 002-conversion.cpp
         * 
         * @brief This is an example of how to use wstring_to_string(). 
         * 
         */

        /**
         * @brief Converts std::string to std::wstring using codepage
         * 
         * If fails, returns empty string, std::wstring(L"")
         * 
         * @param str for string to convert
         * @param codepage for Code Page (default CP_UTF8)
         * @return std::wstring for the converted UTF16 string
         * 
         * @sa <a target ="_blank" href="002-conversion_8cpp_source.html">002-conversion.cpp</a>
         * @include 002-conversion.cpp
         */
        inline std::wstring string_to_wstring(const std::string& str, 
            unsigned int codepage = CP_UTF8)
        {
            if (str.empty()) return L"";

            // https://goo.gl/upmy99
            // Compute the character length of the string
            int wstr_len = MultiByteToWideChar(
                codepage, 0, str.c_str(), (int)str.size(), NULL, 0);

            // if failed to compute the character length
            if (wstr_len == 0) return L"";

            // prepare buffer to hold converted string
            // do not +1 to utf16_len, because std::wstring
            // manages terminating null internally
            std::wstring wstr(wstr_len, L'\0');

            int converted = MultiByteToWideChar(
                codepage, 0, str.c_str(), (int)str.size(),
                &wstr[0], wstr_len);

            return (converted == 0 ? L"": wstr);
        }

        /**
         * @brief Converts UTF16 std::wstring to UTF8 std::string
         * 
         * If fails, returns empty string, std::string("")
         * 
         * @param utf16str for UTF16 std::wstring to convert
         * @return std::string for the converted UTF8 string
         * 
         * @sa <a target ="_blank" href="002-conversion_8cpp_source.html">002-conversion.cpp</a>
         */
        inline std::string utf16_to_utf8(const std::wstring& utf16str)
        {
            if (utf16str.empty()) return "";
            return wstring_to_string(utf16str, CP_UTF8);
        }

        /**
         * @brief Converts UTF8 std::string to UTF16 std::wstring
         * 
         * If fails, returns empty string, std::wstring(L"")
         * 
         * @param utf8str for UTF8 std::string to convert 
         * @return std::wstring for the converted UTF16 string
         * 
         * @sa <a target ="_blank" href="002-conversion_8cpp_source.html">002-conversion.cpp</a>
         */
        inline std::wstring utf8_to_utf16(const std::string& utf8str)
        {
            if (utf8str.empty()) 
                return L"";
            return 
                string_to_wstring(utf8str, CP_UTF8);           
        }

        /**
         * @brief Converts UTF16 (const wchar_t*) to UTF8 (std::string)
         * 
         * This function converts zero terminated UTF16 or const wchar_t*
         * to UTF8 std::string. If conversion fails, returns empty string "".
         *  
         * @param utf16str for UTF16 string to convert to UTF8 
         * @return std::string for UTF8 string, if fails returns empty string ""
         * 
         * @sa <a target ="_blank" href="002-conversion_8cpp_source.html">002-conversion.cpp</a>
         */
        inline std::string utf16_to_utf8(const wchar_t* utf16str)
        {
            if (utf16str == nullptr) return "";

            // count of character of UTF16 string
            int utf16_len = (int)wcslen(utf16str);
            if (utf16_len < 1) return "";

            return utf16_to_utf8(std::wstring(utf16str));
        }

        /**
         * @brief Converts UTF8 string (const char*) to UTF16 (std::wstring)
         * 
         * If fails, returns empty string or L"".
         * 
         * @param utf8str for UTF8 (const char*) to convert 
         * @return std::wstring for the converted UTF16 (std::wstring)
         * 
         * @sa <a target ="_blank" href="002-conversion_8cpp_source.html">002-conversion.cpp</a>
         */
        inline std::wstring utf8_to_utf16(const char* utf8str)
        {
            // if utf8str is nullptr, return empty string L""
            if (utf8str == nullptr) return L"";

            // count of byte of the string to be converted
            int utf8str_len = (int)strlen(utf8str);
            if (utf8str_len < 1) return L"";

            return utf8_to_utf16(std::string(utf8str));
        }

        /**
         * @brief Converts UTF16 wchar_t to UFT8 char.
         * 
         * If fails, returns '\0'
         * 
         * @param utf16_char for UTF16 wchar_t to convert
         * @return char for the converted UTF8 char
         * 
         * @sa <a target ="_blank" href="002-conversion_8cpp_source.html">002-conversion.cpp</a>
         */
        inline char utf16_to_utf8(const wchar_t utf16_char)
        {
            wchar_t src_str[] = {utf16_char, L'\0'};
            auto rlt = utf16_to_utf8(std::wstring(src_str));
            return (rlt.empty() ? '\0': rlt[0]);
        }

        /**
         * @brief Converts UTF8 char to UTF16 char.
         * 
         * If fails, returns L'\0'
         * 
         * @param utf8_char for UTF8 char to convert
         * @return wchar_t for the converted UTF16 character
         * 
         * @sa <a target ="_blank" href="002-conversion_8cpp_source.html">002-conversion.cpp</a>
         */
        inline wchar_t utf8_to_utf16(const char utf8_char)
        {
           char src_str[] = {utf8_char, '\0'};
           auto rlt = utf8_to_utf16(std::string(src_str));
           return (rlt.empty() ? L'\0': rlt[0]);
        }

    }
    //end of namespace conversion
}
// end of namespace mcc

#endif