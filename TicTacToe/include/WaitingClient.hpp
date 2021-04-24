#include "includes.h"
class WaitingClient{
private:
	std::string name;
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
	std::string GetName(){return name;};
};
