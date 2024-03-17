/*****************************************************************//**
 * \file   sequence_terminal.hpp
 * \brief  MFC ������Ʈ ������ �α� ����� ���� ���� �͹̳��� ����
 *         �⺻������ Windows �ܼ��� �̷��� ANSI �̽������� �������� �������� �ʰų�,
 *         ��������� �ش� ����� Ȱ��ȭ
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
    // ���� �ڵ带 �����ϴ� enum
    enum class ErrorCode
    {
      Success = 0,              // ����
      InvalidHandle = -1,       // ��ȿ���� ���� �ڵ�
      GetModeFailed = -2,       // ��� �������� ����
      SetModeFailed = -3        // ��� ���� ����
    };

    static bool is_enabled;
    // �̱��� ���� �޼���
    static sequence_terminal& get_instance() {
      static sequence_terminal instance;
      return instance;
    }

    //static bool is_enabled;
  public:
    sequence_terminal(const sequence_terminal&) = delete;
    sequence_terminal& operator=(const sequence_terminal&) = delete;


    ~sequence_terminal() {
      // �ܼ� â ����
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
        std::cerr << "ANSI �̽������� ������ �͹̳� ó�� Ȱ��ȭ ����" << std::endl;
      else
      {
        std::cout << "�ܼ� â ������. CLI �غ� �Ϸ�." << std::endl;
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
          // �ܼ� â ����
          is_enabled = false;
          FreeConsole();
          std::cout << "�ܼ� â ������." << std::endl;
          // �ܼ� â �ڵ��� ã���ϴ�.
          HWND consoleWnd = GetConsoleWindow();
          if (consoleWnd != NULL) {
            // �ܼ� â�� ������ �ݽ��ϴ�.
            SendMessage(consoleWnd, WM_CLOSE, 0, 0);
          }
        }

  };

}
// end of namespace TinyC


#endif // UTIL_VIRTUAL_TERMINAL_HPP