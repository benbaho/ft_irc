/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurmus <bdurmus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:43:30 by bdurmus           #+#    #+#             */
/*   Updated: 2023/11/15 20:20:13 by bdurmus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "server.hpp"
#include "stdlib.h"
#include "iostream"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "AAAAAAAAAAA" << std::endl;
        return (0);
    }
    std::cout << av[1] << std::endl;
    std::cout << av[2] << std::endl;
    Server mySw(atoi(av[1]), av[2]);
    return (0);
}