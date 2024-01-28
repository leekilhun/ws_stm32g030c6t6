#pragma once
#ifndef _HW_NET_COMM_HPP
#define _HW_NET_COMM_HPP

/**
 * @file net_comm.hpp
 * @author lee kil hun (gns.lee@samsung.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../def.hpp"



#pragma comment(lib, "ws2_32.lib")

#define LOOPBACK_LOCALHOST_IP "127.0.0.1"

namespace hw_socket
{

    using Pool_t = std::pmr::monotonic_buffer_resource;

    enum class NetMode_e : int
    {
        TCP_Server,
        TCP_Client,
        UDP_Server,
        UDP_Client,
    };

    // Concept definition
    template <typename ObjectType>
    concept has_socket_mode_c = requires(ObjectType Obj) {
        typename ObjectType::SocketMode;
    };

    class TcpClient
    {
    public:
        using SocketMode = NetMode_e;
        SocketMode socket_mode;

    private:
        SOCKET m_socket;
        bool m_isConnected;
        struct addrinfo *m_addr;
        std::string m_ip;
        int m_port;

    public:
        TcpClient() : socket_mode{NetMode_e::TCP_Client}, m_socket{}, m_isConnected{}, m_addr{}, m_ip{}, m_port{}
        {
        }

        TcpClient(const std::string &ip_str, int port)
        {
            socket_mode = NetMode_e::TCP_Client;
            InitSocket(ip_str, port);
        }

        ~TcpClient()
        {
            // 소켓을 닫습니다.
            ::freeaddrinfo(m_addr);
            ::closesocket(m_socket);
            // WSACleanup() 함수를 호출하여 Winsock 라이브러리를 정리합니다.
            WSACleanup();
        }

    private:
        void initSocket(const std::string &ip_str, int port)
        {
            m_ip = ip_str;
            m_port = port;
            WSADATA wsaData{};
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // version 2.2
                throw std::runtime_error("gError: Failed to initialize Winsock");

            std::string version{};
            version = "version " + std::to_string(HIBYTE(wsaData.wVersion)) + "." + std::to_string(LOBYTE(wsaData.wVersion));
            std::cout << "> wsaData.wVersion : " << version << std::endl;
            version = "version " + std::to_string(HIBYTE(wsaData.wHighVersion)) + "." + std::to_string(LOBYTE(wsaData.wHighVersion));
            std::cout << "> wsaData.wHighVersion : " << version << std::endl;
            std::cout << "> wsaData.szSystemStatus : " << wsaData.szSystemStatus << std::endl;

            struct addrinfo hints = {};
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;
            int status = getaddrinfo(ip_str.c_str(), std::to_string(port).c_str(), &hints, &m_addr);
            if (status != 0)
            {
                throw std::runtime_error("getaddrinfo failed: ");  //+ std::string(gai_strerror(status))
            }
            m_socket = socket(m_addr->ai_family, m_addr->ai_socktype, m_addr->ai_protocol);
            if (m_socket == INVALID_SOCKET)
            {
                throw std::runtime_error("socket creation failed: " + std::to_string(WSAGetLastError()));
            }
        }

        void connect(const std::string &ip, int port)
        {
            if (::connect(m_socket, m_addr->ai_addr, (int)m_addr->ai_addrlen) == SOCKET_ERROR)
            {
                ::closesocket(m_socket);
                throw std::runtime_error("connection failed: " + std::to_string(WSAGetLastError()));
            }
        }

        void closeSocket()
        {
            ::closesocket(m_socket);
            m_isConnected = false;
        }

        int send(const std::vector<char> &datas)
        {
            if (!m_isConnected)
                return -1;     
            return ::send(m_socket, datas.data(), (int)datas.size(), 0);
        }

        int receive(std::vector<char> &ret_vec)
        {
            char buffer[1024];
            int ret_flag{};
            int bytesReceived = ::recv(m_socket, buffer, sizeof(buffer), ret_flag);
            if (bytesReceived)
            {
                {
                    //TODO::
                }

                for (int i = 0; i < bytesReceived; ++i)
                {
                    ret_vec.emplace_back(buffer[i]);
                }
                ret_vec.shrink_to_fit();
            }
            return bytesReceived;
        }

        int receive(std::pmr::vector<char> &pool_vec)
        {
            int ret_flag{};
            int bytesReceived = ::recv(m_socket, pool_vec.data(), sizeof(pool_vec.capacity()), ret_flag); // waiting block
            if (bytesReceived)
            {
                {
                    // TODO::
                }
            }

            return (int)pool_vec.size();
        }

    public:
        bool InitSocket(const std::string &ip_str, int port)
        {
            try
            {
                initSocket(ip_str, port);
                return true;
            }
            catch (std::exception &e)
            {
                std::cerr << "error: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool Connect()
        {
            try
            {
                connect(m_ip, m_port);
                m_isConnected = true;
                return true;
            }
            catch (std::exception &e)
            {
                std::cerr << "error: " << e.what() << std::endl;
                return false;
            }
        }

        void Close()
        {
            closeSocket();
        }

        template <typename DataType>
        int Send(const DataType &datas)
        {
            if constexpr (std::is_same_v<DataType, std::vector<char>>)
            {
                //std::cout << "Send Vector Type Datas" << std::endl;
               return send(datas);
            }
            else if constexpr (std::is_same_v<DataType, std::string>)
            {
                //std::cout << "Send String Type Datas" << datas << std::endl;
                return send(std::vector<char>{datas.begin(), datas.end()});
            }
            else
            {
                using type_of = decltype(datas);
                std::cout << "Send Data Fail!" << std::endl;
                return -1;
            }

        }

        template <typename DataType>
        void Receive(DataType &vbuf) // std::vector<char>
        {

            if constexpr (std::is_same_v<DataType, std::vector<char>>)
            {
                int rec_length = receive(vbuf);
            }
            else if constexpr (std::is_same_v<DataType, std::pmr::vector<char>>)
            {
                int rec_length = receive(vbuf);
            }
        }
    };
    // end of   class TcpClient
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TcpServer
    {
    public:
        using SocketMode = NetMode_e;
        SocketMode socket_mode;

    private:
        struct st_client
        {
            sockaddr_storage their_addr{};
            SOCKET client_socket{};
        } m_clientSocket;
        SOCKET m_socket;
        bool m_isOpened;
        bool m_isConnectedClient;
        struct addrinfo *m_addr;
        std::string m_ip;
        int m_port;

    public:
        TcpServer() : m_clientSocket{}, socket_mode{NetMode_e::TCP_Server}, m_socket{}, m_isOpened{}, m_isConnectedClient{}, m_addr{}, m_ip{}, m_port{}
        {
        }
        TcpServer(const std::string &ip_str, int port)
        {
            socket_mode = NetMode_e::TCP_Server;
            initSocket(ip_str, port);
        }

        ~TcpServer()
        {
            ::freeaddrinfo(m_addr);
            ::closesocket(m_socket);
            ::WSACleanup();
        }

    private:
        void initSocket(const std::string &ip_str, int port)
        {
            WSADATA wsaData{};
            m_port = port;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // version 2.2
                throw std::runtime_error("gError: Failed to initialize Winsock");

            std::string version{};
            version = "version " + std::to_string(HIBYTE(wsaData.wVersion)) + "." + std::to_string(LOBYTE(wsaData.wVersion));
            std::cout << "> wsaData.wVersion : " << version << std::endl;
            version = "version " + std::to_string(HIBYTE(wsaData.wHighVersion)) + "." + std::to_string(LOBYTE(wsaData.wHighVersion));
            std::cout << "> wsaData.wHighVersion : " << version << std::endl;
            std::cout << "> wsaData.szSystemStatus : " << wsaData.szSystemStatus << std::endl;

            struct addrinfo hints = {};
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;
            int status = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &m_addr);
            if (status != 0)
            {
                throw std::runtime_error("getaddrinfo failed: "); // + std::string(gai_strerror(status))  //unicode set. 
            }
            m_socket = socket(m_addr->ai_family, m_addr->ai_socktype, m_addr->ai_protocol);
            if (m_socket == INVALID_SOCKET)
            {
                throw std::runtime_error("socket creation failed: " + std::to_string(WSAGetLastError()));
            }
            //////////////////////////////////
        }

        void bindListen()
        {
            if (bind(m_socket, m_addr->ai_addr, (int)m_addr->ai_addrlen) == SOCKET_ERROR)
            {
                closesocket(m_socket);
                throw std::runtime_error("bind failed: " + std::to_string(WSAGetLastError()));
            }

            freeaddrinfo(m_addr);

            if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
            {
                closesocket(m_socket);
                throw std::runtime_error("listen failed: " + std::to_string(WSAGetLastError()));
            }
        }

        int send(const std::vector<char> &datas)
        {
            if (!m_isConnectedClient)
                return -1;                
              return (::send(m_clientSocket.client_socket, datas.data(), (int)datas.size(), 0) == SOCKET_ERROR);
        }

    public:
        bool InitSocket(const std::string &ip_str, int port)
        {
            // Winsock 라이브러리를 초기화합니다.

            try
            {
                initSocket(ip_str, port);
                return true;
            }
            catch (std::exception &e)
            {
                std::cerr << "error: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool OpenServer()
        {
            try
            {
                bindListen();
                std::cout << "waiting for connection..." << std::endl;
                m_isOpened = true;
                return true;
            }
            catch (std::exception &e)
            {
                std::cerr << "error: " << e.what() << std::endl;
                return false;
            }
        }

        void Run()
        {

            socklen_t addr_size = sizeof(m_clientSocket.their_addr);
            while (true)
            {
                std::cout << "wait accept port " << std::endl;
                m_clientSocket.client_socket = ::accept(m_socket, (sockaddr *)&m_clientSocket.their_addr, &addr_size);
                if (m_clientSocket.client_socket == INVALID_SOCKET)
                {
                    throw std::runtime_error("accept failed: " + std::to_string(WSAGetLastError()));
                }
                m_isConnectedClient = true;
                while (true)
                {
                    std::cout << "wait data receive" << std::endl;
                    char buf[1024];
                    int num_bytes = ::recv(m_clientSocket.client_socket, buf, 1024, 0);
                    if (num_bytes == SOCKET_ERROR)
                    {
                        throw std::runtime_error("recv failed: " + std::to_string(WSAGetLastError()));
                    }
                    if (num_bytes == 0)
                    {
                        std::cout << "client disconnected" << std::endl;
                        break;
                    }
                    std::string recv_data(buf, num_bytes);
                    {
                        //TODO::
                    }
                  //  std::cout << "data received : " << recv_data << std::endl;
                }
                m_isConnectedClient = false;

                ::closesocket(m_clientSocket.client_socket);
            }
            // while (m_isOpened)
        }

        bool IsClientConnected() const
        {
            return m_isConnectedClient;
        }

        void Close()
        {
            m_isOpened = false;

            ::freeaddrinfo(m_addr);
            ::closesocket(m_socket);
            ::WSACleanup();
        }

        template <typename DataType>
        int Send(const DataType &datas)
        {
            if constexpr (std::is_same_v<DataType, std::vector<char>>)
            {
                return send(datas);
            }
            else if constexpr (std::is_same_v<DataType, std::string>)
            {
               return send(std::vector<char>{datas.begin(), datas.end()});
            }
            else
            {
                using type_of = decltype(datas);
                std::cout << "Send Data Fail!" << std::endl;
                return -1;
            }
        }
    };
    // end of   class TcpServer

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class UdpClient
    {
    public:
        using SocketMode = NetMode_e;
        SocketMode socket_mode;

    private:
        SOCKET m_socket;
        bool m_isConnected;
        struct sockaddr_in m_addr;
        std::string m_ip;
        int m_port;

    public:
        UdpClient() : socket_mode{NetMode_e::UDP_Client}, m_socket(), m_isConnected{}, m_addr{}, m_ip{}, m_port{}
        {
        }

        UdpClient(const std::string &host, int port)
        {
            socket_mode = NetMode_e::UDP_Client;
            initSocket(host, port);
        }

        ~UdpClient()
        {
            // 소켓을 닫습니다.
            ::closesocket(m_socket);
            // WSACleanup() 함수를 호출하여 Winsock 라이브러리를 정리합니다.
            WSACleanup();
        }

    private:
        void initSocket(const std::string &ip_str, int port)
        {
            m_ip = ip_str;
            m_port = port;
            WSADATA wsaData{};
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // version 2.2
                throw std::runtime_error("gError: Failed to initialize Winsock");

            m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (m_socket == INVALID_SOCKET)
            {
                WSACleanup();
                throw std::runtime_error("socket creation failed: " + std::to_string(WSAGetLastError()));
            }
        }

        void connect(const std::string &ip, int port)
        {
            memset(&m_addr, 0, sizeof(m_addr));
            m_addr.sin_family = AF_INET;
            m_addr.sin_port = ::htons(port);

            if (::inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr) == SOCKET_ERROR)
            {
                ::closesocket(m_socket);
                throw std::runtime_error("connection failed: " + std::to_string(WSAGetLastError()));
            }
            m_isConnected = true;
        }

        void closeSocket()
        {
            ::closesocket(m_socket);
            m_isConnected = false;
            WSACleanup();
        }

        int send(const std::vector<char> &datas)
        {
            if (!m_isConnected)
                return -1;
            // 데이터를 송신합니다.
            return (::sendto(m_socket, datas.data(), (int)datas.size(), 0, (struct sockaddr *)&m_addr, sizeof(m_addr)) == SOCKET_ERROR);
        }

        int receive(std::vector<char> &ret_vec)
        {
            char buffer[1024];
            int ret_flag{};
            int size = sizeof(m_addr);
            int bytesReceived = ::recvfrom(m_socket, buffer, 1024, 0, (struct sockaddr *)&m_addr, &size);
            if (bytesReceived)
            {

                for (int i = 0; i < bytesReceived; ++i)
                {
                    ret_vec.emplace_back(buffer[i]);
                }
                ret_vec.shrink_to_fit();
            }
            return bytesReceived;
        }

    public:
        bool InitSocket(const std::string &ip_str, int port)
        {
            // Winsock 라이브러리를 초기화합니다.
            try
            {
                initSocket(ip_str, port);
                return true;
            }
            catch (std::exception &e)
            {
                std::cerr << "error: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool Connect()
        {
            try
            {
                connect(m_ip, m_port);
                return true;
            }
            catch (std::exception &e)
            {
                std::cerr << "error: " << e.what() << std::endl;
                return false;
            }
        }

        void Close()
        {
            closeSocket();
        }

        template <typename DataType>
        int Send(const DataType &datas)
        {
            int ret{};
            if constexpr (std::is_same_v<DataType, std::vector<char>>)
            {
                //std::cout << "Send Vector Type Datas" << std::endl;
                ret = send(datas);
                if (ret == ERROR_SUCCESS)
                    ret = (int)datas.size();
                return ret;
            }
            else if constexpr (std::is_same_v<DataType, std::string>)
            {
                //std::cout << "Send String Type Datas" << datas << std::endl;
                ret = send(std::vector<char>{datas.begin(), datas.end()});
                if (ret == ERROR_SUCCESS)
                    ret = (int)datas.size();
                return ret;
            }
            else
            {
                using type_of = decltype(datas);
                std::cout << "Send Data Fail!" << std::endl;
                return -1;
            }
        }

        template <typename DataType>
        void Receive(DataType &vbuf) // std::vector<char>
        {

            if constexpr (std::is_same_v<DataType, std::vector<char>>)
            {
                /*
                constexpr std::size_t Count = 1024;
                constexpr std::size_t ByteCount = sizeof(char) * Count + 1024;
                char buffer[ByteCount]{};         // stack memory
                Pool_t pool{ buffer, ByteCount }; // std::pmr::monotonic_buffer_resource
                */
                int rec_length = receive(vbuf);
            }
            else if constexpr (std::is_same_v<DataType, std::pmr::vector<char>>)
            {
                int rec_length = receive(vbuf);
            }
        }
    };
    // end of UdpClient

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class UdpServer
    {
    public:
        using SocketMode = NetMode_e;
        SocketMode socket_mode;

    private:
        SOCKET m_socket;
        bool m_isOpened;
        sockaddr_in m_addr;
        sockaddr_in m_their_addr;
        std::string m_ip;
        int m_port;

    public:
        UdpServer() : socket_mode{NetMode_e::UDP_Server}, m_socket(), m_isOpened{}, m_addr{}, m_their_addr{}, m_ip{}, m_port{}
        {
        }

        UdpServer(const std::string &ip_str, int port)
        {
            socket_mode = NetMode_e::UDP_Server;
            initSocket(ip_str, port);
        }
        ~UdpServer()
        {
            // Clean up
            ::closesocket(m_socket);
            WSACleanup();
        }

    private:
        void initSocket(const std::string &ip_str, int port)
        {
            WSADATA wsaData{};
            m_port = port;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // version 2.2
                throw std::runtime_error("gError: Failed to initialize Winsock");

            std::string version{};
            version = "version " + std::to_string(HIBYTE(wsaData.wVersion)) + "." + std::to_string(LOBYTE(wsaData.wVersion));
            std::cout << "> wsaData.wVersion : " << version << std::endl;
            version = "version " + std::to_string(HIBYTE(wsaData.wHighVersion)) + "." + std::to_string(LOBYTE(wsaData.wHighVersion));
            std::cout << "> wsaData.wHighVersion : " << version << std::endl;
            std::cout << "> wsaData.szSystemStatus : " << wsaData.szSystemStatus << std::endl;

            // Create UDP socket
            m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (m_socket == INVALID_SOCKET)
            {
                throw std::runtime_error("socket creation failed: " + std::to_string(WSAGetLastError()));
            }
            //////////////////////////////////
        }

        void bind()
        {
            memset(&m_addr, 0, sizeof(m_addr));
            m_addr.sin_family = AF_INET;
            m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            m_addr.sin_port = htons(m_port);

            if (::bind(m_socket, (struct sockaddr *)&m_addr, sizeof(m_addr)) == SOCKET_ERROR)
            {
                ::closesocket(m_socket);
                throw std::runtime_error("bind failed: " + std::to_string(WSAGetLastError()));
            }
        }

        int send(const std::vector<char> &datas)
        {
            if (!m_their_addr.sin_port)
                return -1;
            return (sendto(m_socket, datas.data(), (int)datas.size(), 0, (struct sockaddr *)&m_their_addr, sizeof(m_their_addr)) == SOCKET_ERROR);
        }

        errno_t send(const std::string &data)
        {
            if (!m_isOpened)
                return -1;
            // 데이터를 송신합니다.

            if (sendto(m_socket, data.c_str(), (int)data.length(), 0, (struct sockaddr *)&m_their_addr, sizeof(m_their_addr)) == SOCKET_ERROR)
            {
                std::cerr << "Error sending message" << std::endl;
                return -1;
            }

            return ERROR_SUCCESS;
        }

    public:
        bool InitSocket(const std::string &ip_str, int port)
        {
            // Winsock 라이브러리를 초기화합니다.

            try
            {
                initSocket(ip_str, port);
                return true;
            }
            catch (std::exception &e)
            {
                std::cerr << "error: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        bool OpenServer()
        {
            try
            {
                bind();
                std::cout << "bind .." << std::endl;
                m_isOpened = true;

                return true;
            }
            catch (std::exception &e)
            {
                std::cerr << "error: " << e.what() << std::endl;
                return false;
            }
        }

        void Run()
        {

            while (m_isOpened)
            {
                char buffer[1024]{};
                int client_address_len = sizeof(m_their_addr);

                // Receive log message
                int num_bytes = recvfrom(m_socket, buffer, 1024, 0, (struct sockaddr *)&m_their_addr, &client_address_len);

                if (num_bytes == -1)
                {
                    m_their_addr.sin_port = 0;
                    std::cerr << "Error receiving message" << std::endl;
                }
                else
                {
                    // Print log message to console
                    buffer[num_bytes % 1024] = '\0';
                    char addr_str[INET_ADDRSTRLEN]{};
                    {
                        //TODO:
                    }
                    //std::cout << "Received log message from " << inet_ntop(AF_INET, &m_their_addr.sin_addr, addr_str, INET_ADDRSTRLEN) << ": " << buffer << std::endl;
                }
            }
            // while ()
        }

        void Close()
        {
            m_isOpened = false;

            ::closesocket(m_socket);
            WSACleanup();
        }

        template <typename DataType>
        int Send(const DataType &datas)
        {
            if constexpr (std::is_same_v<DataType, std::vector<char>>)
            {
                return send(datas);
            }
            else if constexpr (std::is_same_v<DataType, std::string>)
            {
                return send(std::vector<char>{datas.begin(), datas.end()});
            }
            else
            {
                using type_of = decltype(datas);
                std::cout << "Send Data Fail!" << std::endl;
                return -1;
            }
        }
    };
    // /end of   class UdpServer

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
// end of namespace hw_socket

namespace hw_socket
{

    template <typename ClassType>
        requires has_socket_mode_c<ClassType>
    class NetComm : public IComm
    {
    public:
        struct cfg_t
        {
            std::string ip_str{};
            uint16_t port_no{};

            cfg_t() = default;
            cfg_t &operator=(const cfg_t &other)
            {
                ip_str = other.ip_str;
                port_no = other.port_no;
                return *this;
            }
        };

    public:
        cfg_t m_cfg;
        ClassType::SocketMode m_SocketMode;

    private:
        /* data */
        //TinyC::Que<uint8_t> m_Que;
        std::unique_ptr<ClassType> m_instance;
        bool m_isOpenPort;
        std::thread m_trd;
        std::atomic<bool> m_stopThread;
        std::function<int(void *, void *)> m_cb;

    public:
        NetComm() : m_cfg{}, m_SocketMode{ClassType{}.socket_mode}, m_instance{std::make_unique<ClassType>()}
        ,  m_isOpenPort{}, m_trd{}, m_stopThread{}, m_cb{}
        {
           //m_SocketMode = ClassType{}.socket_mode;
           //m_instance = std::make_unique<ClassType>();
        }

        ~NetComm()
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
            std::string recv_str{};

            constexpr std::size_t Count = 1024;
            constexpr std::size_t ByteCount = sizeof(char) * Count + 1024;

            char buffer[ByteCount]{}; // stack memory
            // std::pmr::monotonic_buffer_resource pool{buffer, ByteCount};
            Pool_t pool{buffer, ByteCount};
            std::pmr::vector<char> ret_buf{&pool};
            std::vector<char> rec_v{};
            m_instance->Receive(rec_v);
            if (rec_v.size() > 0)
            {
                {
                    //TODO:
                    if (m_cb)
                    {
                        int size = (int)rec_v.size(); 
                        m_cb(&size, rec_v.data());
                    }
                    else
                    {
                        for (const char &elm : rec_v)
                        {
                          //  m_Que.Put((uint8_t)elm);
                        }
                    }
                }
                 //std::cout << "received data : " << std::string{rec_v.begin(), rec_v.end()} <<std::endl;
            }
        }

        inline void threadRun()
        {
            std::string str{"this Socket mode is "};
            str.append(GetMode());
            m_instance->Send(str);
            std::cout << "Thread Start!" << std::endl;

            if constexpr (std::is_same_v<ClassType, TcpClient>)
            {
                while (!m_stopThread)
                {
                    threadJob();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            else if constexpr (std::is_same_v<ClassType, TcpServer>)
            {
                while (!m_stopThread)
                {
                    m_instance->Run();
                }
            }
            else if constexpr (std::is_same_v<ClassType, UdpClient>)
            {
                while (!m_stopThread)
                {
                    threadJob();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            else if constexpr (std::is_same_v<ClassType, UdpServer>)
            {
                while (!m_stopThread)
                {
                    m_instance->Run();
                }
            }

            std::cout << "Thread Terminatet!" << std::endl;
        }

    public:
        inline std::string GetMode() const
        {
            switch (m_SocketMode)
            {
            case NetMode_e::TCP_Server:
                return "TCP_Server";
            case NetMode_e::TCP_Client:
                return "TCP_Client";
            case NetMode_e::UDP_Server:
                return "UDP_Server";
            case NetMode_e::UDP_Client:
                return "UDP_Client";

            default:
                return "";
            }
        }

        inline bool InitSocket(cfg_t cfg) //
        {
            m_cfg = cfg;

            m_instance->InitSocket(cfg.ip_str, cfg.port_no);
            return true;
        }

        inline bool Connect()
        {

            if constexpr (std::is_same_v<ClassType, TcpServer>
            || std::is_same_v<ClassType, UdpServer>)
            {
                if (m_instance->OpenServer())
                {
                    m_isOpenPort = true;
                    m_trd = std::thread(&NetComm::threadRun, this);
                    return true;
                }
            }            
            else if constexpr (std::is_same_v<ClassType, TcpClient>
            || std::is_same_v<ClassType, UdpClient>)
            {
                if (m_instance->Connect())
                {
                    m_isOpenPort = true;
                    m_trd = std::thread(&NetComm::threadRun, this);
                    return true;
                }
            }           

            return false;
        }

        inline bool IsConnected() const
        {
            if constexpr (std::is_same_v<ClassType, TcpServer>)
            {
                return m_instance->IsClientConnected();
            }
            else if constexpr (std::is_same_v<ClassType, UdpServer>)
            {
                return m_isOpenPort;
            }
            else if constexpr (std::is_same_v<ClassType, TcpClient>)
            {
                return m_isOpenPort;
            }
            else if constexpr (std::is_same_v<ClassType, UdpClient>)
            {
                return m_isOpenPort;
            }
        }

        inline int SendData(const char *ptr_data, size_t length)
        {
            std::vector<char> datas;
            for (size_t i = 0; i < length; i++)
            {
                datas.emplace_back(ptr_data[i]);
            }
            return m_instance->Send(datas);
        }

        inline errno_t Open()
        {
            if (Connect())
                return ERROR_SUCCESS;

            return -1;
        }

        inline void Close()
        {
            m_instance->Close();
            threadStop();
        }

        inline void registerCallback(std::function<int(void *, void *)> cb)
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
                        std::cout << "> net instance address  : [" << this << "] " << std::endl;
                        std::cout << "> net mode \t\t: [" << this->GetMode() << "] " << std::endl;
                        std::cout << "> net ip  and port \t: [" << this->m_cfg.ip_str << "] ["  << std::to_string(this->m_cfg.port_no) << "] "<< std::endl;
                        std::cout << "> net status \t\t: [" << (this->IsConnected() ? std::string{"connected"} : std::string{"disconnected"}) << "] " << std::endl;                        
                        ret = true;
                    }
                    else if (std::string(argv[1]).compare("close") == 0)
                    {
                        this->Close();
                        std::cout << "net close success" << std::endl;
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
                        std::string ip_str(argv[2]);
                        uint32_t port = (uint32_t)get_data(argv[3]);
#if 0
                        if (this->open_port((const char *)port_name.c_str(), baudrate) == ERROR_SUCCESS)
                            std::cout << "uart open [" << argv[2] << "] [" << argv[3] << "] success" << std::endl;
                        else
                            std::cout << "uart open [" << argv[2] << "] [" << argv[3] << "] failed !" << std::endl;
#endif
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

            std::cout << "net info" << std::endl;
            std::cout << "net close" << std::endl;
            std::cout << "net open [ip_str] [port_no]" << std::endl;

            return -1;
        }
    };
    // end of class NetComm

}
// end of namespace hw_socket



#endif