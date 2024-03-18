/*****************************************************************//**
 * \file   sequence_terminal.hpp
 * \brief  Creates a virtual terminal for log output when creating an MFC project
 *         By default, Windows console does not support these ANSI escape sequences,
 *         or explicitly enables this feature
 * \author gns2l
 * \date   March 2024
 *********************************************************************/

#pragma once
#ifndef UTIL_SEQUENCE_TERMINAL_HPP
#define UTIL_SEQUENCE_TERMINAL_HPP


#include <iostream>
#include <windows.h>

namespace TinyC
{

  class sequence_terminal
  {
  public:
    // Enum defining error codes
    enum class ErrorCode
    {
      Success = 0,              // Success
      InvalidHandle = -1,       // Invalid handle
      GetModeFailed = -2,       // Failed to get mode
      SetModeFailed = -3        // Failed to set mode
    };

    static bool is_enabled;
    // Singleton access method
    static sequence_terminal& get_instance() {
      static sequence_terminal instance;
      return instance;
    }

    //static bool is_enabled;
  public:
    sequence_terminal(const sequence_terminal&) = delete;
    sequence_terminal& operator=(const sequence_terminal&) = delete;


    ~sequence_terminal() {
      // Release console window
      if (sequence_terminal::is_enabled)
        disable_sequence_terminal_processing();
    };

  public:sequence_terminal() {
    if (AllocConsole())
    {
      freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
      freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
      freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);

      if (enable_sequence_terminal_processing() != ErrorCode::Success)
        std::cout << "Failed to enable ANSI escape sequence terminal processing" << std::endl;
      else
      {
        std::cout << "Console window created. CLI ready." << std::endl;
        // is_enabled = true;
      }
    }
  }

        static ErrorCode enable_sequence_terminal_processing()
        {
          HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
          if (hOut == INVALID_HANDLE_VALUE)
            return ErrorCode::InvalidHandle;

          DWORD dwMode = 0;
          if (!GetConsoleMode(hOut, &dwMode))
            return ErrorCode::GetModeFailed;

          dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
          if (!SetConsoleMode(hOut, dwMode))
            return ErrorCode::SetModeFailed;


          return ErrorCode::Success;
        }

        static void disable_sequence_terminal_processing()
        {
          // Release console window
          is_enabled = false;
          FreeConsole();
          std::cout << "Console window released." << std::endl;
          // Find the console window handle.
          HWND consoleWnd = GetConsoleWindow();
          if (consoleWnd != NULL) {
            // Force close the console window.
            SendMessage(consoleWnd, WM_CLOSE, 0, 0);
          }
        }

  };

}
// end of namespace TinyC


#endif // UTIL_VIRTUAL_TERMINAL_HPP