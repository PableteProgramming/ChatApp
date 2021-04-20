#include <ClientClass.hpp>

ClientClass::ClientClass(int _id){
	id=_id;
	exit=false;
	reading= new std::thread(Read,&exit,id);
}

void Read(bool* exit,int id){
	while(!(*exit)){
		char receivedMessage[1024] = {0};
		read( id , receivedMessage, sizeof(receivedMessage));	
		std::string message(receivedMessage);
		if(message=="exit"){
			(*exit)=true;
		}
		else{
			if(message!=""){	
				std::cout<<message<<std::endl;
			}
		}	
	}
}
