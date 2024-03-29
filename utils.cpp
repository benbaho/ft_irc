#include "Server.hpp"

std::string    strLower(const std::string &str)
{
    std::string lowerStr(str);

    for (std::string::iterator it = lowerStr.begin(); it != lowerStr.end(); it++)
    {
        if (isalpha(*it))
            *it = tolower(*it);
        else if (*it == '[')
            *it = '{';
        else if (*it == ']')
            *it = '}';
        else if (*it == '\\')
            *it = '|';
    }
    return(lowerStr);
}

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

// MODE 

std::string getMaskNick(std::string mask)
{
    std::stringstream           ss(mask);
    std::string                 nick;
    std::string::size_type      pos = mask.rfind('!');

    if (pos != std::string::npos)
        nick = mask.substr(0, pos);
    else
        nick = mask;
    return(nick.empty() ? std::string("*") : nick);
}
