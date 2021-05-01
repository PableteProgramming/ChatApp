#include "includes.h"

#ifdef __linux__
    std::string SocketRead(int);
    void ClientRead(int, bool*);
    void SocketSend(int, std::string);
#else
    std::string SocketRead(SOCKET);
    void ClientRead(SOCKET, bool*);
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
	void JoinThread();
};

#ifdef __linux__
    void WaitingClientRead(int, bool*);
#else
	void WaitingClientRead(SOCKET, bool*);
#endif