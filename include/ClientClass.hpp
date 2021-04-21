#include "includes.h"

void Read(bool*,int, std::string);

class ClientClass{
private:
	std::string name;
	std::thread* reading;
	bool exit;	
	int id;
public:
	ClientClass(int, std::string);
	bool GetExit(){return exit;};
};
