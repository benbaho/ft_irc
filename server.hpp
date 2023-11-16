/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurmus <bdurmus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:43:35 by bdurmus           #+#    #+#             */
/*   Updated: 2023/11/15 20:06:33 by bdurmus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "iostream"

class Server
{
    public:
        
        Server(int port, char *pass);
        
        int         serverFd;
        std::string serverPass;
        
        void createSocket(int port);
        void serverLoop();

};