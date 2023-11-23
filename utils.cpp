#include "Server.hpp"
#include <sstream>

std::string stringTrim(char *buff)
{
    try
    {
        std::string res(buff);
        return res.substr(res.find_first_not_of("\r\n\t"), res.find_last_not_of("\r\n\t") + 1);
    }
    catch(std::exception &e){}
    return "";
}

void    printDec(std::string str)
{
    for (int i = 0; str[i]; i++) 
    {
        if (!isprint(str[i]))
            std::cout << static_cast<int>(str[i]) << std::flush;
        else
            std::cout << str[i] << std::flush;
    }
    std::cout << "\n" << std::flush;
}
