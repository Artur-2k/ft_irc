/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:06:28 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:08:31 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserService.hpp"
#include <cerrno>
#include <cstring>

UserService* UserService::_instance = NULL;
/* 
    THIS CLASS AIMS TO MANAGE AND IMPLEMENT METHODS FOR
    ALL USERS OF THE PROGRAM, WITH THE OBJECTIVE OF SEPARATING AND/OR
    DECOUPLING THE LOGIC FOR GREATER SCALABILITY, EASY MAINTENANCE, AND 
    VERSATILITY WHEN CREATING NEW COMMANDS OR IMPLEMENTING NEW 
    FEATURES.

    USE CASES: WHEN CREATING A COMMAND, ALL COMMANDS HAVE POINTERS TO
    THIS CLASS, WHERE YOU CAN SIMPLY REUSE THE METHODS AND/OR 
    IMPLEMENT NEW ONES THAT WILL FACILITATE/SIMPLIFY FUTURE LOGIC.
 */

UserService::UserService() {}

UserService::~UserService() {}


/* 
    WHEN A NEW CLIENT CONNECTS, IT SIMPLY ALLOCATES A NEW USER
    AND ASSIGNS IT TO THE MAP <FD, USER*>. WHERE ITS FD IS REGISTERED, 
    SINCE THE NICKNAME WILL ONLY BE ASSIGNED WHEN USING THE NICK COMMAND
 */
void UserService::CreateUserByFd(int fd)
{
    _usersByFd[fd] = new User(fd);
}

const std::map<int, User*> UserService::getFdsMap() const
{
    return this->_usersByFd;
}


UserService& UserService::getInstance()
{
    if (!_instance)
        _instance = new UserService();
    return *_instance;
}


/* 
    METHOD TO DELETE A USER INSTANCE, CLOSE THE CONNECTION FD,
    AND REMOVE REFERENCES IN THE FD MAP AND NICKNAME MAP, IF THEY EXIST
 */
void UserService::RemoveUserByFd(int fd)
{
    std::map<int, User*>::iterator it;
    std::map<std::string, User*>::iterator itNick;
    
    it = _usersByFd.find(fd);
    

    // Erase both maps and delete the User
    if (it != _usersByFd.end())
    {
        itNick = _userByNick.find(it->second->getNick());
        if (itNick != _userByNick.end())
            _userByNick.erase(itNick);
        if (it->second)
            delete it->second;
        _usersByFd.erase(it);
    }
    close(fd); // close user socket
    

}

/* 
    METHOD TO DELETE A USER INSTANCE, CLOSE THE CONNECTION FD,
    AND REMOVE REFERENCES IN THE NICKNAME MAP AND SUBSEQUENTLY 
    IN THE FD MAP
 */
void UserService::RemoveUserByNick(std::string nickname)
{
    std::map<std::string, User*>::iterator it;
    std::map<int, User*>::iterator itFd;

    it = _userByNick.find(nickname);

    if (it != _userByNick.end())
    {
        itFd = _usersByFd.find(it->second->getFd());
        close(it->second->getFd()); // fecha o fd do user
        _userByNick.erase(it);
        delete itFd->second;
        _usersByFd.erase(itFd);
    }
}

/* 
    METHOD TO SEARCH FOR A USER USING FD.
    IF IT EXISTS, RETURNS A POINTER TO THE USER INSTANCE.
    IF NOT, RETURNS NULL.
 */
User* UserService::findUserByFd(int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    if (it != _usersByFd.end())
        return it->second;
    else
        return NULL;
}

/* 
    METHOD TO SEARCH FOR A USER USING NICKNAME.
    IF IT EXISTS, RETURNS A POINTER TO THE USER INSTANCE.
    IF NOT, RETURNS NULL.
 */
User* UserService::findUserByNickname(std::string nickname)
{
    std::map<std::string, User*>::iterator it;
    it = _userByNick.find(nickname);

    if (it != _userByNick.end())
        return it->second;
    else 
        return NULL; 
}

/* 
    METHOD TO SET THE NICKNAME OF A USER USING ONLY THE FD
 */
void UserService::SetNickByFd(std::string nickname, int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    if (it != _usersByFd.end())
    {
        // This bit is when we are replacing nickname. We must remove the entry of the last one
        std::map<std::string, User*>::iterator it2;
        it2 = _userByNick.find(it->second->getNick());
        if (it2 != _userByNick.end())
            _userByNick.erase(it2);
        
        it->second->setNick(nickname); // map<int, User*>
        _userByNick[nickname] = it->second; // map<string, User*>
    }
    else
        return ;
}

/* 
    METHOD TO SET THE NICKNAME OF A USER USING ONLY THE FD,
 */
void UserService::SetUserByFd(std::string username, int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);


    if (it != _usersByFd.end())
        it->second->setUser(username);
    else
        return ; 
}


void UserService::SetRealNameByFd(std::string realName, int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    if (it != _usersByFd.end())
        it->second->setRealName(realName);
    else
        return ; 
}


void UserService::sendMessage(int fd, const std::string &message)
{
    if (fd < 0)
    {
        std::cerr << "[ERROR]: Invalid file descriptor" << std::endl;
        return;
    }

    std::string formattedMessage = message + "\r\n";
    ssize_t bytesSent = send(fd, formattedMessage.c_str(), formattedMessage.size(), MSG_NOSIGNAL);

    if (bytesSent == -1)
    {
        std::cerr << "[ERROR]: Failed to send message to fd " << fd << ": " << strerror(errno) << std::endl;
    }
}


// c++ -Wall -Wextra -Werror -std=c++98 UserService.cpp ../Models/User.cpp 


/* int main(void)
{

    UserService service;

    service.CreateUserByFd(1);
    service.SetNickByFd("TIGRINHO", 1);
    service.SetUserByFd("TIGRINHO_USER", 1);

    service.CreateUserByFd(2);
    service.SetNickByFd("TIGRINHO2", 2);
    service.SetUserByFd("TIGRINHO2_USER", 2);


    std::cout << "TESTES COM RESULTADO POSITIVO-------------" << std::endl << std::endl;

    std::cout << std::endl << "TESTE FIND POR FD" << std::endl;

    User* teste1 = service.findUserByFd(1);

    User* teste2 = service.findUserByNickname("TIGRINHO2");

    std::cout << "-NICK DO USER: " << teste1->getNick() << std::endl;

    std::cout << std::endl << "TESTE FIND POR NICK" << std::endl;

    teste2 = service.findUserByNickname("TIGRINHO2");

    std::cout << "-NICK DO USER: " << teste2->getNick() << std::endl;

    std::cout << std::endl << "TESTES COM RESULTADO NEGATIVO-------------" << std::endl << std::endl;
    
    std::cout << std::endl << "TESTE DELETE USER PELO FD EXISTENTE" << std::endl;
    
    std::cout << "-User existe? " << teste1 << std::endl;
    
    service.RemoveUserByFd(1);

    teste1 = service.findUserByFd(1);

    std::cout << "-Depois de deletar ainda existe? " << teste1 << std::endl;

    std::cout << std::endl << "TESTE DELETE USER PELO NICK EXISTENTE" << std::endl;

    std::cout << "-User existe? " << teste2 << std::endl;

    service.RemoveUserByNick("TIGRINHO2");

    teste2 = service.findUserByNickname("TIGRINHO2");

    std::cout << "-Depois de deletar ainda existe? " << teste2 << std::endl;


    std::cout << std::endl << "TESTE DELETE USER INEXISTENTE" << std::endl;

    service.RemoveUserByFd(3);

    teste1 = service.findUserByFd(3);

    std::cout << "-User existe? " << teste1 << std::endl;
        
} */
