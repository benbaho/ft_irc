#include "Server.hpp"
#include "Client.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
        return (0);
    Server myServer(atoi(av[1]), av[2]);
    return (0);
}