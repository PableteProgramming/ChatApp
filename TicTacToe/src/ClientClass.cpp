#include <ClientClass.hpp>

ClientClass::ClientClass(int _id, std::string _name){
	id=_id;
	name = _name;
	exit=false;
	reading= new std::thread(Read,&exit,id, name);
}

void Read(bool* exit,int id, std::string name){
	while(!(*exit)){
		char receivedMessage[1024] = {0};
		read( id , receivedMessage, sizeof(receivedMessage));	
		std::string message(receivedMessage);
		if(message=="exit"){
			(*exit)=true;
		}
		else{
			if(message!=""){	
				std::string toSend = "\033[34m[" + name + "] > \033[0m" + message;;
				std::cout<<toSend<<std::endl;
			}
		}	
	}
}
