#pragma once
#ifndef _HW_SERIAL_COMM_HPP
#define _HW_SERIAL_COMM_HPP

/**
 * @file serial_comm.hpp
 * @author lee kil hun (gns.lee@samsung.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../def.hpp"

class serial_comm
{
public:
    struct cfg_t
    {
        std::string port_name{};
        uint32_t baudrate{};

        cfg_t() = default;
        cfg_t& operator=(const cfg_t& other)
        {
            port_name = other.port_name;
            baudrate = other.baudrate;
            return *this;
        }
    } m_cfg;

private:
    /* data */
    TinyC::Que<uint8_t> m_Que;
    HANDLE m_hSerial;
    bool m_isOpenPort;
    std::thread m_trd;
    std::atomic<bool> m_stopThread;
    std::function<int(void*, void*)> m_cb;

public:
    serial_comm(/* args */) : m_cfg{}, m_Que{ 4096 }, m_hSerial{}, m_isOpenPort{}, m_trd{}, m_stopThread{}, m_cb{}
    {
    }
    ~serial_comm()
    {
        if (m_isOpenPort)
        {
            Close();
        }
    }

private:
    inline void threadStop()
    {
        m_stopThread = true;
        m_trd.join();
    }

    inline void threadJob()
    {

        constexpr int SERIAL_COMM_BUFF_MAX = 4096;
        int32_t length = 0;
        DWORD dwRead = 0;
        std::array<uint8_t, SERIAL_COMM_BUFF_MAX> arry_buff{};

        if (ReadFile(m_hSerial, (LPVOID)arry_buff.data(), (DWORD)SERIAL_COMM_BUFF_MAX, &dwRead, NULL) == TRUE)
        {
            if (dwRead)
            {
                if (m_cb)
                {
                    m_cb(&dwRead, arry_buff.data());
                }
                else
                {
                    for (uint8_t& e : arry_buff)
                    {
                        m_Que.Put(e);
                    }
                }
            }
        }
    }

    inline void threadRun()
    {
        std::cout << "Thread Start!" << std::endl;
        while (!m_stopThread)
        {
            threadJob();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Thread Terminatet!" << std::endl;
    }

    inline errno_t open_port(const char* port_name, uint32_t baud)
    {
        errno_t err_code = 0;

        if(m_isOpenPort)
        {
            return ERROR_SUCCESS;
        }

        constexpr int SERIAL_COMM_BUFF_MAX = 4096;
        DCB dcb{};
        COMMTIMEOUTS timeouts;
        DWORD dwError;

        char name[MAX_PATH] = {
            0,
        };
        sprintf_s(name, MAX_PATH, "//./%s", port_name);

        m_hSerial = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_hSerial == INVALID_HANDLE_VALUE)
        {
            return (errno_t)GetLastError();
        }

        while (1)
        {
            dcb.DCBlength = sizeof(DCB);
            if (GetCommState(m_hSerial, &dcb) == FALSE)
                goto gotoErrOut;

            // Set baudrate
            dcb.BaudRate = (DWORD)baud;
            dcb.ByteSize = 8;          // Data bit = 8bit
            dcb.Parity = NOPARITY;     // No parity
            dcb.StopBits = ONESTOPBIT; // Stop bit = 1
            dcb.fParity = NOPARITY;    // No Parity check
            dcb.fBinary = 1;           // Binary mode
            dcb.fNull = 0;             // Get Null byte
            dcb.fAbortOnError = 0;
            dcb.fErrorChar = 0;
            // Not using XOn/XOff
            dcb.fOutX = 0;
            dcb.fInX = 0;
            // Not using H/W flow control
            dcb.fDtrControl = DTR_CONTROL_DISABLE;
            dcb.fRtsControl = RTS_CONTROL_DISABLE;
            dcb.fDsrSensitivity = 0;
            dcb.fOutxDsrFlow = 0;
            dcb.fOutxCtsFlow = 0;

            if (SetCommState(m_hSerial, &dcb) == FALSE)
                goto gotoErrOut;

            if (SetCommMask(m_hSerial, 0) == FALSE) // Not using Comm event
                goto gotoErrOut;

            if (SetupComm(m_hSerial, SERIAL_COMM_BUFF_MAX, SERIAL_COMM_BUFF_MAX) == FALSE) // Buffer size (Rx,Tx)
                goto gotoErrOut;

            if (PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR) == FALSE) // Clear buffer
                goto gotoErrOut;

            if (ClearCommError(m_hSerial, &dwError, NULL) == FALSE)
                goto gotoErrOut;

            if (GetCommTimeouts(m_hSerial, &timeouts) == FALSE)
                goto gotoErrOut;

#define NOT_USE_SERIAL_COMM_TIMEOUT

#ifdef NOT_USE_SERIAL_COMM_TIMEOUT
            // Timeout (Not using timeout)
            // Immediatly return
            timeouts.ReadIntervalTimeout = 0;
            timeouts.ReadTotalTimeoutMultiplier = 0;
            timeouts.ReadTotalTimeoutConstant = 1; // must not be zero.
            timeouts.WriteTotalTimeoutMultiplier = 0;
            timeouts.WriteTotalTimeoutConstant = 0;
#else
            /*
             The maximum time allowed to elapse before the arrival of the next byte on the communications line, in milliseconds.
             If the interval between the arrival of any two bytes exceeds this amount
             */
            timeouts.ReadIntervalTimeout = 50;
            /*
             A constant used to calculate the total time-out period for read operations, in milliseconds
            */
            timeouts.ReadTotalTimeoutConstant = 50;
            /*
              The multiplier used to calculate the total time-out period for read operations, in milliseconds.
             */
            timeouts.ReadTotalTimeoutMultiplier = 10;
            /*
            A constant used to calculate the total time-out period for write operations, in milliseconds.
            */
            timeouts.WriteTotalTimeoutConstant = 50;
            /*
            The multiplier used to calculate the total time-out period for write operations, in milliseconds.
            */
            timeouts.WriteTotalTimeoutMultiplier = 10;
#endif

            if (SetCommTimeouts(m_hSerial, &timeouts) == FALSE)
                goto gotoErrOut;

            EscapeCommFunction(m_hSerial, SETRTS);
            EscapeCommFunction(m_hSerial, SETDTR);
            break;

        gotoErrOut:
            err_code = (errno_t)GetLastError();
            break;
        }

        if (err_code != 0)
        {
            CloseHandle(m_hSerial);
        }
        else
        {
            SetCommMask(m_hSerial, EV_RXCHAR);
            // threadRun();
            m_isOpenPort = true;

            m_trd = std::thread(&serial_comm::threadRun, this);
        }

        return err_code;
    }

    inline bool closePort()
    {
        m_isOpenPort = false;
        return CloseHandle(m_hSerial);
    }

    inline std::string wstring_to_string(const std::wstring& wstr,
        unsigned int codepage = CP_UTF8)
    {
        if (wstr.empty())
            return "";

        int str_len = WideCharToMultiByte(
            codepage,
            0, // dwFlag
            wstr.c_str(),
            (int)wstr.size(), NULL, 0, NULL, NULL);

        // if failed to compute the byte count of the string
        if (str_len == 0)
            return "";

        // prepare a string buffer to
        // hold the converted string
        // do not +1 to str_len,
        // because std::string manages terminating null
        std::string str(str_len, '\0');

        int converted = WideCharToMultiByte(
            codepage, 0, wstr.c_str(), (int)wstr.size(),
            &str[0], str_len, NULL, NULL);

        return (converted == 0 ? "" : str);
    }

    inline std::vector<std::string> getSerialPorts()
    {
        std::vector<std::string> serialPorts;
        wchar_t lpTargetPath[5000];
        for (int i = 0; i < 255; i++)
        {
            std::wstring wstr = L"COM" + std::to_wstring(i);
            DWORD test = QueryDosDeviceW(wstr.c_str(), lpTargetPath, 5000);
            if (test != 0)
            {
                serialPorts.push_back(wstring_to_string(wstr));
            }
        }
        return serialPorts;
    }

public:
    inline errno_t Init(serial_comm::cfg_t cfg)
    {
        m_cfg = cfg;
        return open_port((const char*)m_cfg.port_name.c_str(), m_cfg.baudrate);
    }

    inline bool IsOpen() const
    {
        return m_isOpenPort;
    }

    inline errno_t OpenPort()
    {
        return open_port((const char*)m_cfg.port_name.c_str(), m_cfg.baudrate);
    }

    

    inline uint8_t Read()
    {
        uint8_t ret;
        m_Que.Get(&ret);
        return ret; /*m_received_data*/
        ;
    }

    inline uint32_t Write(uint8_t* p_data, uint32_t length)
    {
        uint32_t ret = 0;
        DWORD dwWrite = 0;

        if (WriteFile(m_hSerial, p_data, (DWORD)length, &dwWrite, NULL) == FALSE)
        {
            ret = 0;
        }
        else
        {
            ret = dwWrite;
        }

        return ret;
    }

    inline errno_t Open() 
    {
        return OpenPort();
    }

    inline void Close()
    {
        threadStop();
        closePort();
    }

    inline bool IsConnected() const 
    {
        return IsOpen();
    }

    inline int SendData(const char* ptr_data, uint32_t length)
    {
        DWORD dwWrite = 0;
        int ret = 0;
        if (WriteFile(m_hSerial, ptr_data, (DWORD)length, &dwWrite, NULL) == FALSE)
        {
            ret = 0;
        }
        else
        {
            ret = dwWrite;
        }

        return ret;
    }

    inline void registerCallback(std::function<int(void*, void*)> cb)
    {
        m_cb = cb;
    }

    /****************************************************
     *	test code
     ****************************************************/
    inline int testCB_func(int argc, char *argv[])
    {
        auto get_data = [](char *int_ptr) -> int
        {
            int ret = 0;
            ret = (int)strtoul((const char *)int_ptr, (char **)NULL, (int)0);
            return ret;
        };

        bool ret{};
        enum : int
        {
            arg_cnt_0,
            arg_cnt_1,
            arg_cnt_2,
            arg_cnt_3
        };
        switch ((argc - 1))
        {
        case arg_cnt_1:
        {
            if (argv[1])
            {
                if (std::string(argv[1]).compare("info") == 0)
                {
                    std::cout << "uart info [" << this << "] success " << std::endl;
                    ret = true;
                }
                else if (std::string(argv[1]).compare("search") == 0)
                {
                    std::vector<std::string> ports = this->getSerialPorts();
                    if (ports.empty())
                        std::cout << "uart search success but not searched any port" << std::endl;
                    else
                    {
                        for (auto const& elm : ports)
                        {
                            std::cout << elm << std::endl;
                        }
                    }
                    ret = true;
                }
                else if (std::string(argv[1]).compare("close") == 0)
                {
                    this->Close();
                    std::cout << "uart close success" << std::endl;
                    ret = true;
                }
            }
        }
        break;
        case arg_cnt_2:
        {
        }
        break;
        case arg_cnt_3:
        {
            if (argv[1])
            {
                if (std::string(argv[1]).compare("open") == 0)
                {
                    std::string port_name(argv[2]);
                    uint32_t baudrate = (uint32_t)get_data(argv[3]);

                    if (this->open_port((const char*)port_name.c_str(), baudrate) == ERROR_SUCCESS)
                        std::cout << "uart open [" << argv[2] << "] [" << argv[3] << "] success" << std::endl;
                    else
                        std::cout << "uart open [" << argv[2] << "] [" << argv[3] << "] failed !" << std::endl;
                    ret = true;
                }
            }
        }
        break;
        default:
            break;
        }
        // end of switch

        if (ret)
            return 0;

        std::cout << "uart info" << std::endl;
        std::cout << "uart search" << std::endl;
        std::cout << "uart close" << std::endl;
        std::cout << "uart open [name] [buadrate]" << std::endl;

        return -1;
    }
};

#endif 
//__SERIAL_COMM_H__