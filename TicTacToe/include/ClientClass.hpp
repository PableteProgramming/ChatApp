#include "includes.h"
class ClientClass;
void Read(bool*,int, std::string,ClientClass*);

class ClientClass{
private:
	std::string name;
	std::thread* reading;
	ClientClass* Friend;
	int id;
public:
	bool exit;
	ClientClass(int, std::string);
	std::string GetName(){return name;};
	int GetId(){return id;};
	void StartThread(ClientClass*);
};
