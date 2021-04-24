#include "ClientClass.hpp"
#include "WaitingClient.hpp"
const int PORT = 8080;
const std::string add= "127.0.0.1";


#ifdef __linux__
    std::string SocketRead(int);
    void ClientRead(int, bool*);
    void SocketSend(int, std::string);
#else
    std::string SocketRead(SOCKET);
    void ClientRead(SOCKET, bool*);
    void SocketSend(SOCKET, std::string);
#endif
