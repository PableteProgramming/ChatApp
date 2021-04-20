#include "includes.h"

void Read(bool*,int);

class ClientClass{
private:
	std::thread* reading;
	bool exit;	
	int id;
public:
	ClientClass(int);
	bool GetExit(){return exit;};
};
