#include <WaitingClient.hpp>

#ifdef __linux__
	WaitingClient::WaitingClient(int _id,std::string _name)
#else
	WaitingClient::WaitingClient(SOCKET _id,std::string _name)
#endif
{
	id=_id;
	name=_name;
	reading = NULL;
	waiting = true;
}

void WaitingClient::StartThread()
{
	reading = new std::thread(WaitingClientRead, this);
}

void WaitingClientRead(WaitingClient* client){
	//std::cout << "Starting thread..." << std::endl;
	while(client->waiting){
		//std::cout << "Waiting message" << std::endl;
		std::string message = SocketRead(client->GetId());
		if(message=="exit"){
			client->waiting=false;
			//std::cout<<"Client wants to exit form the waiting room"<<std::endl;
		}
		else{
			//std::cout << message << std::endl;
		}
	}
	//std::cout<<"Waiting room finished"<<std::endl;
}