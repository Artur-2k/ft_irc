/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:12:41 by fang              #+#    #+#             */
/*   Updated: 2025/03/24 16:08:15 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

/**
 * This class uses a static instance of Server that has its pros an cons
 * and serves the purpose of letting the program have the server configs
 * wherever needed 
 * Usage: Server::getInstance() -> will return the static instance
 */

class Server
{
private:
    Server();
    ~Server();

    std::string _password;
    static const std::string _serverName;
    
public:

    static Server& getInstance();
    void setPassword(std::string password);
    std::string getPassword() const;
    const std::string getServerName() const;
};

