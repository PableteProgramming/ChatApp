#include "includes.h"
#include "Player.hpp"
#ifdef __linux__
    std::string SocketRead(int);
    void ClientRead(int, bool*, Player*);
    void SocketSend(int, std::string);
#else
    std::string SocketRead(SOCKET);
    void ClientRead(SOCKET, bool*, Player*);
    void SocketSend(SOCKET, std::string);
#endif

class WaitingClient{
private:
	std::string name;
	std::thread* reading;
#ifdef __linux__
	int id;
public:
	WaitingClient(int,std::string);
	int GetId(){return id;};
#else
	SOCKET id;
public:
	WaitingClient(SOCKET,std::string);
	SOCKET GetId(){return id;};
#endif
public:
	bool waiting;
	std::string GetName(){return name;};
	void StartThread();
};

void WaitingClientRead(WaitingClient*);
