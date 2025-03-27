/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:46:23 by fang              #+#    #+#             */
/*   Updated: 2025/03/27 16:29:35 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//Includes
#include <unistd.h> // close
#include <cstdlib> // exit
#include <limits> // numeric limits
#include <iostream> // cout cerr endl
#include <cerrno> // errno
#include <cstring> // strerror
#include <sys/socket.h> // socket connect send recv
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // in_addr_t and inet_addr

class Bot
{
    private:
        const std::string _botName = "UselessBot";
        const std::string _botServerPass = "d082dc29b65ed2e80db4ef542ca7ee8c4d3971043cbe214090f61b238493893f";

        int _botSocketFd;
    public:
        //Default Constructor
        Bot( void );
        // Name param Constructor
        Bot(const std::string& botName);
        //Destructor
        ~Bot( void );

        static bool IsPortValid(const char *port);
        
        bool Connect(const in_addr_t ipAddr, const unsigned short port);
        void Register( void );
        void RecieveData( void );
        void HandleData( void );
        void ExecuteCommands( void );
        
        
};