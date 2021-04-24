#include <main.hpp>
#include "includes.h"

#ifdef __linux__
    std::string SocketRead(int _socket)
    {
        char recievedMessage[1024] = { 0 };
        read(_socket, recievedMessage, sizeof(recievedMessage));
        return std::string(recievedMessage);
    }
    void SocketSend(int _socket, std::string _message)
    {
		send(_socket , _message.c_str() , strlen(_message.c_str()) , 0 );
    }   
#else
    std::string SocketRead(SOCKET _socket)
    {
        char recievedMessage[1024] = { 0 };
        recv(_socket, recievedMessage, sizeof(recievedMessage), 0);
        return std::string(recievedMessage);
    }
    void SocketSend(SOCKET _socket, std::string _message)
    {
		send(_socket , _message.c_str() , strlen(_message.c_str()) , 0 );
    }
#endif