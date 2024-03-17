/*****************************************************************//**
 * \file   sequence_terminal.hpp
 * \brief  MFC 프로젝트 생성시 로그 출력을 위한 가상 터미널을 생성
 *         기본적으로 Windows 콘솔은 이러한 ANSI 이스케이프 시퀀스를 지원하지 않거나,
 *         명시적으로 해당 기능을 활성화
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
    // 오류 코드를 정의하는 enum
    enum class ErrorCode
    {
      Success = 0,              // 성공
      InvalidHandle = -1,       // 유효하지 않은 핸들
      GetModeFailed = -2,       // 모드 가져오기 실패
      SetModeFailed = -3        // 모드 설정 실패
    };

    static bool is_enabled;
    // 싱글톤 접근 메서드
    static sequence_terminal& get_instance() {
      static sequence_terminal instance;
      return instance;
    }

    //static bool is_enabled;
  public:
    sequence_terminal(const sequence_terminal&) = delete;
    sequence_terminal& operator=(const sequence_terminal&) = delete;


    ~sequence_terminal() {
      // 콘솔 창 해제
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
        std::cerr << "ANSI 이스케이프 시퀀스 터미널 처리 활성화 실패" << std::endl;
      else
      {
        std::cout << "콘솔 창 생성됨. CLI 준비 완료." << std::endl;
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
          // 콘솔 창 해제
          is_enabled = false;
          FreeConsole();
          std::cout << "콘솔 창 해제됨." << std::endl;
          // 콘솔 창 핸들을 찾습니다.
          HWND consoleWnd = GetConsoleWindow();
          if (consoleWnd != NULL) {
            // 콘솔 창을 강제로 닫습니다.
            SendMessage(consoleWnd, WM_CLOSE, 0, 0);
          }
        }

  };

}
// end of namespace TinyC


#endif // UTIL_VIRTUAL_TERMINAL_HPP