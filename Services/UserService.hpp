/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:07:28 by davi              #+#    #+#             */
/*   Updated: 2025/03/17 17:24:16 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Models/User.hpp"
#include "../Utils/Utils.hpp"

#include "unistd.h"
#include <iostream>
#include <map>

/* 
    ESSA CLASSE TEM O PROPOSITO DE GERENCIAR E IMPLEMENTAR METHODOS PARA
    TODAS OS USUARIOS DO PROGRAMA, ONDE TEM COMO OBJETIVO, SEPARAR E/OU
    DESACOPLAR AS LOGICAS PARA MAIS ESCALABILIDADE, FACIL MANUTENCAO E 
    VERSATILIDADE NA HORA DE CRIAR NOVOS COMANDOS OU IMPLEMENTAR NOVAS 
    FEATURES

    USE CASES: AO CRIAR UM COMANDO, TODOS COMANDOS POSSUEM PONTEIROS PARA
    ESSA CLASSE, ONDE VOCE PODE SIMPLESMENTE REUTILIZAR OS METODOS E/OU 
    IMPLEMENTAR NOVOS QUE IRAO FACILITAR/SIMPLICAR LOGICAS FUTURAS
 */

class UserService
{
private:
    // Infelizmente redundante porem otimizado para buscas
    std::map<int, User*> _usersByFd;
    std::map<std::string, User*> _userByNick;

public:
    UserService();
    ~UserService();

    User* findUserByFd(int fd);
    User* findUserByNickname(std::string nickname);

     // Nickname etc setados posteriormente com o fd usando os metodos do service
    void CreateUserByFd(int fd);
    void RemoveUserByFd(int fd);
    void RemoveUserByNick(std::string username);

    void SetNickByFd(std::string nickname, int fd);
    void SetUserByFd(std::string username, int fd);
    void SetRealNameByFd(std::string realName, int fd);

};