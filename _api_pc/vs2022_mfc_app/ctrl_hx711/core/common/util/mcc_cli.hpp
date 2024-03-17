/*****************************************************************//**
 * \file   mcc_cli.hpp
 * \brief
 *         콘솔 입출력을 이용한 간단한 CLI 라이브러리
 *         만들어 봅니다.
 *
 * \author lee kil hun (gns.lee2@samsung.com)
 * \date   March 2024
 *********************************************************************/

#pragma once
#ifndef _MCC_CLI_HPP
#define _MCC_CLI_HPP


#include <sstream>
#include <conio.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
#include <future>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>
#include "sequence_terminal.hpp"


#ifndef NOMINMAX
#define NOMINMAX
#endif


constexpr const char* cmd_color_ResetAll = "\033[0m";
constexpr const char* cmd_color_Bold = "\033[1m";
constexpr const char* cmd_color_Dim = "\033[2m";
constexpr const char* cmd_color_Underlined = "\033[4m";
constexpr const char* cmd_color_Blink = "\033[5m";
constexpr const char* cmd_color_Reverse = "\033[7m";
constexpr const char* cmd_color_Hidden = "\033[8m";
constexpr const char* cmd_color_ResetBold = "\033[21m";
constexpr const char* cmd_color_ResetDim = "\033[22m";
constexpr const char* cmd_color_ResetUnderlined = "\033[24m";
constexpr const char* cmd_color_ResetBlink = "\033[25m";
constexpr const char* cmd_color_ResetReverse = "\033[27m";
constexpr const char* cmd_color_ResetHidden = "\033[28m";
constexpr const char* cmd_color_Default = "\033[39m";
constexpr const char* cmd_color_Black = "\033[30m";
constexpr const char* cmd_color_Red = "\033[31m";
constexpr const char* cmd_color_Green = "\033[32m";
constexpr const char* cmd_color_Yellow = "\033[33m";
constexpr const char* cmd_color_Blue = "\033[34m";
constexpr const char* cmd_color_Magenta = "\033[35m";
constexpr const char* cmd_color_Cyan = "\033[36m";
constexpr const char* cmd_color_LightGray = "\033[37m";
constexpr const char* cmd_color_DarkGray = "\033[90m";
constexpr const char* cmd_color_LightRed = "\033[91m";
constexpr const char* cmd_color_LightGreen = "\033[92m";
constexpr const char* cmd_color_LightYellow = "\033[93m";
constexpr const char* cmd_color_LightBlue = "\033[94m";
constexpr const char* cmd_color_LightMagenta = "\033[95m";
constexpr const char* cmd_color_LightCyan = "\033[96m";
constexpr const char* cmd_color_White = "\033[97m";
constexpr const char* cmd_color_BackgroundDefault = "\033[49m";
constexpr const char* cmd_color_BackgroundBlack = "\033[40m";
constexpr const char* cmd_color_BackgroundRed = "\033[41m";
constexpr const char* cmd_color_BackgroundGreen = "\033[42m";
constexpr const char* cmd_color_BackgroundYellow = "\033[43m";
constexpr const char* cmd_color_BackgroundBlue = "\033[44m";
constexpr const char* cmd_color_BackgroundMagenta = "\033[45m";
constexpr const char* cmd_color_BackgroundCyan = "\033[46m";
constexpr const char* cmd_color_BackgroundLightGray = "\033[47m";
constexpr const char* cmd_color_BackgroundDarkGray = "\033[100m";
constexpr const char* cmd_color_BackgroundLightRed = "\033[101m";
constexpr const char* cmd_color_BackgroundLightGreen = "\033[102m";
constexpr const char* cmd_color_BackgroundLightYellow = "\033[103m";
constexpr const char* cmd_color_BackgroundLightBlue = "\033[104m";
constexpr const char* cmd_color_BackgroundLightMagenta = "\033[105m";
constexpr const char* cmd_color_BackgroundLightCyan = "\033[106m";
constexpr const char* cmd_color_BackgroundWhite = "\033[107m";

namespace TinyC
{
  class Cli
  {
  private:
    /* data */
    static std::atomic<bool> trd_life;
    static HANDLE hConsole;
    static std::string c_input;
    static std::map<std::string, std::function<int(int, char**)>> callbacks;

    static std::thread task_trd;
  public:
    Cli(/* args */)
    {
      Cli::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    ~Cli()
    {
      threadStop();
    }

    static void threadStop()
    {
      trd_life = false;
      if (task_trd.joinable())
      {
        sequence_terminal::disable_sequence_terminal_processing();
        task_trd.join();
      }
    }

    static void threadJob()
    {
      std::cout << "cli Thread Start!" << std::endl;
      while (trd_life)
      {
        TinyC::Cli::cli_main();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      std::cout << " cli Thread Terminatet!" << std::endl;
    }

    static void threadRun()
    {
      registerCallback("help", TinyC::Cli::cli_show_list);
      trd_life = true;
      task_trd = std::thread(&TinyC::Cli::threadJob);
    }

  public:

    static inline void registerCallback(std::string key, std::function<int(int, char**)> cb)
    {
      callbacks[key] = cb;
    }

    static inline int split(char* arg_str, char** argv, const char* delim_chars, std::size_t max = 1024)
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

    static inline void parseLine(std::string& line)
    {
      constexpr std::size_t split_max = 256;
      const char delim_chars[] = " ";
      char* argv[split_max] = {};
      int argc = split((char*)line.c_str(), argv, delim_chars, split_max);

      if (argc < 1)
        return;
      else
      {
        if (argv[0])
        {
          std::string name_str(argv[0]);
          const auto& it = callbacks.find(name_str);
          if (it != callbacks.end())
          {
            it->second((argc), argv);
          }
        }
      }
    }

    static inline std::string cli_to_upper(std::string_view str)
    {
      std::string s(str);
      std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c)
        { return std::toupper(c); });

      return s;
    }

    static inline bool cliKeepLoop(void)
    {
      // char c{};
      // std::cin.get(c);

      if (_kbhit())
        return false;
      else
        return true;
    }

    static inline int cli_show_list(int argc, char* argv[])
    {
      std::cout << "---------- Test List ---------" << std::endl;
      for (const auto& [key, value] : callbacks)
      {
        std::cout << " " << cli_to_upper(key) << std::endl;
      }
      std::cout << "------------------------------" << std::endl;

      return 0;
    }

    static inline void cli_main(void)
    {

      std::cout << cmd_color_Green << " #cli > " << cmd_color_ResetAll; // 녹색 프롬프트 출력
      std::getline(std::cin, c_input);                                  // 문자열 입력 받기

      if (c_input.size() == 0)
      {
        return;
      }
      parseLine(c_input);
    }
  };
}
// end of namespace TinyC

#endif
// end of _MCC_CLI_HPP