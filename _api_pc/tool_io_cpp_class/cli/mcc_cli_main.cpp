#include "mcc_cli.hpp"
#include "../hw/serial_comm.hpp"
/*

The cli class is used globally
Static members are initialized in the code main code page, 
and the necessary macros are defined and used globally.

*/

// initializer static memvers
TinyC::Cli ap_cli;
bool TinyC::Cli::trd_life = true;
HANDLE TinyC::Cli::hConsole = nullptr;
std::string TinyC::Cli::c_input{};
std::map<std::string, std::function<int(int, char**)>> TinyC::Cli::callbacks{};


// ########## test class ################

class MotorC
{
    std::string m_name;

public:
    MotorC() : m_name{} {};
    ~MotorC() {}
    // copy constructor
    MotorC(const MotorC& other) : m_name(other.m_name) {}

    // move constructor
    MotorC(MotorC&& other) noexcept : m_name(std::move(other.m_name)) {}

    // copy assignment operator
    MotorC& operator=(const MotorC& other)
    {
        m_name = other.m_name;
        return *this;
    }

    // move assignment operator
    MotorC& operator=(MotorC&& other) noexcept
    {
        m_name = std::move(other.m_name);
        return *this;
    }

    std::string GetName() const
    {
        return m_name;
    }

    void SetName(std::string& name)
    {
        m_name = name;
    }

    int callback_func(int argc, char* argv[])
    {
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
                    std::cout << "motor info [" << this << "] success " << std::endl;
                    ret = true;
                }
                else if (std::string(argv[1]).compare("on") == 0)
                {
                    std::cout << "motor on success" << std::endl;
                    ret = true;
                }
                else if (std::string(argv[1]).compare("off") == 0)
                {
                    std::cout << "motor off success" << std::endl;
                    ret = true;
                }
            }
        }
        break;
        case arg_cnt_2:
        {
            if (argv[1])
            {
                if (std::string(argv[1]).compare("jog") == 0)
                {
                    std::cout << "motor jog [" << argv[2] << "] success" << std::endl;
                    ret = true;
                }
            }
        }
        break;
        case arg_cnt_3:
        {
            if (argv[1])
            {
                if (std::string(argv[1]).compare("run") == 0)
                {
                    std::cout << "motor run [" << argv[2] << "] [" << argv[3] << "] success" << std::endl;
                    ret = true;
                }
            }
        }
        break;
        default:
            break;
        }
        //end of switch

        if (ret)
            return 0;


        std::cout << "motor info" << std::endl;
        std::cout << "motor on" << std::endl;
        std::cout << "motor off" << std::endl;
        std::cout << "motor jog CW(-1)/CCW(1)" << std::endl;
        std::cout << "motor run pos vel" << std::endl;

        return -1;
    }

    // std::function<std::string(int,double,std::string)> callback;
};
// end of class



int main()
{

    /* "cmd_str" "func_name" .... */
    ap_cli.registerCallback("help", ap_cli.cli_show_list);

    MotorC motot1;
    MotorC motot2;
    MotorC motot3;
    ap_cli.registerCallback("motor1", std::bind(&MotorC::callback_func, &motot1, std::placeholders::_1, std::placeholders::_2));
    ap_cli.registerCallback("motor2", std::bind(&MotorC::callback_func, &motot2, std::placeholders::_1, std::placeholders::_2));
    ap_cli.registerCallback("motor3", std::bind(&MotorC::callback_func, &motot3, std::placeholders::_1, std::placeholders::_2));

    serial_comm uart;
    ap_cli.registerCallback("uart", std::bind(&serial_comm::testCB_func, &uart, std::placeholders::_1, std::placeholders::_2));


   
    std::cout << "start " << std::endl;
    while (1)
    {
        ap_cli.cli_main();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "terminate " << std::endl;
    return 0;
}