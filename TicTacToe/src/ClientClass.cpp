#include <ClientClass.hpp>

ClientClass::ClientClass(int _id, std::string _name,std::string _friendName){
	id=_id;
	name = _name;
	friendName=_friendName;
	exit=false;
	reading= new std::thread(Read,&exit,id, name,friendName);
}
