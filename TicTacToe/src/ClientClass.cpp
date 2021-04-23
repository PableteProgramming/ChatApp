#include <ClientClass.hpp>

ClientClass::ClientClass(int _id, std::string _name){
	id=_id;
	name = _name;
	exit=false;
	reading= NULL;//new std::thread(Read,&exit,id, name,friendName);
}

void ClientClass::StartThread(ClientClass* FriendClass){
	Friend= FriendClass;
	std::cout<<name<<" => "<<Friend->GetName()<<std::endl;
	reading= new std::thread(Read,&exit,id,name,Friend);
}
