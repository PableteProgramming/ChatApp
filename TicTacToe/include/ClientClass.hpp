#include "includes.h"
class ClientClass;
void Read(ClientClass*);

class ClientClass{
private:
	std::string name;
	std::thread* reading;
	ClientClass* Friend;
#ifdef __linux__
	int id;
public:
	ClientClass(int, std::string);
	int GetId(){return id;};
#else
	SOCKET id;
public:
	ClientClass(SOCKET, std::string);
	SOCKET GetId(){return id;};
#endif
	bool exit;
	std::string GetName(){return name;};
	void StartThread(ClientClass*);
	ClientClass* GetFriend(){return Friend;};
};
