#include "includes.h"
class WaitingClient{
private:
	int id;
	std::string name;
public:
	WaitingClient(int,std::string);
	std::string GetName(){return name;};
	int GetId(){return id;};
};
