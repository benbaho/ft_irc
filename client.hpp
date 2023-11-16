/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurmus <bdurmus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:43:45 by bdurmus           #+#    #+#             */
/*   Updated: 2023/11/15 18:43:47 by bdurmus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "iostream"
#include "vector"

class Client
{
    public:
        Client();
        int cliFd;
        std::string nick;
        int auth;
        std::vector<std::string> incomingMessages;
};