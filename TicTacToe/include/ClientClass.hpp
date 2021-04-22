#include "includes.h"

void Read(bool*,int, std::string,std::string);

class ClientClass{
private:
	std::string name;
	std::thread* reading;
	std::string friendName;
	bool exit;	
	int id;
public:
	ClientClass(int, std::string,std::string);
	bool GetExit(){return exit;};
	std::string GetName(){return name;};
	int GetId(){return id;};
};
