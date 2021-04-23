#include <main.hpp>
std::vector<ClientClass*> clients;
std::vector<WaitingClient> waitingroom;

void ExitClients(){
	while(true){
		std::vector<int> pending;
		pending.clear();
		for(int i=0; i<clients.size();i++){
			if(clients[i]->exit){
				pending.push_back(i);
			}
		}
		for(int i=0; i<pending.size();i++){
			std::cout<<"client "<<clients[pending[i]]->GetName()<<" exits!"<<std::endl;
			clients.erase(clients.begin()+pending[i]);
			std::cout<<clients.size()<<std::endl;
		}
	}
}

int main(){
	int server_fd, client;
	struct sockaddr_in socketObj;
	int opt = 1;
	int socketObjSize = sizeof(socketObj);
	std::string message = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	socketObj.sin_family = AF_INET;
	socketObj.sin_addr.s_addr = INADDR_ANY;
	socketObj.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&socketObj, 
				 sizeof(socketObj))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	
	bool running = true;
	std::thread exitclients(ExitClients);
	while (running)
	{
		client = accept(server_fd, (struct sockaddr *)&socketObj, (socklen_t*)&socketObjSize);
		if (client < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		else
		{
			message = "\033[32mWelcome to the server!\033[0m";
			send(client , message.c_str() , strlen(message.c_str()) , 0 );
			char receivedMessage[1024] = {0};
			read( client , receivedMessage, sizeof(receivedMessage));	
			std::string nameMessage(receivedMessage);
			bool UsernameExists=false;
			for(int i=0; i<clients.size();i++){
				if(clients[i]->GetName()==nameMessage){
					UsernameExists=true;
					break;
				}
			}

			if(UsernameExists){
				send(client,"Y",strlen("Y"),0);				
			}
			else{
				send(client,"N",strlen("N"),0);
				memset(receivedMessage,0,1024);
				read(client, receivedMessage,sizeof(receivedMessage));
				std::string isWaiting(receivedMessage);
				if(isWaiting=="Y"){
					waitingroom.push_back(WaitingClient(client,nameMessage));
				}
				else{
					send(client,"T",strlen("T"),0);
					char friendName[1024]={0};
					std::cout<<"Waiting friend name"<<std::endl;
					read(client,friendName,sizeof(friendName));
					std::cout<<"Lets see what appends..."<<std::endl;
					std::string sFriendName(friendName);
					bool friendNameExists=false;
					int index=-1;
					for(int i=0;i<waitingroom.size();i++){
						if(waitingroom[i].GetName()==sFriendName){
							friendNameExists=true;
							index=i;
							break;
						}
					}
					if(friendNameExists){
						std::cout<<"Friend name exists"<<std::endl;
						send(client,"Y",strlen("Y"),0);
						std::cout<<"Client "<<nameMessage<<" accepted"<<std::endl;
						std::cout<<"Client "<<waitingroom[index].GetName()<<" is now active"<<std::endl;

						ClientClass* temp= new ClientClass(waitingroom[index].GetId(),waitingroom[index].GetName());
						ClientClass* newclient= new ClientClass(client,nameMessage);
						send(temp->GetId(),"T",strlen("T"),0);
						clients.push_back(temp);
						clients.push_back(newclient);
						temp->StartThread(newclient);
						newclient->StartThread(temp);
						waitingroom.erase(waitingroom.begin()+index);

					}
					else{
						send(client,"N",strlen("N"),0);
					}
				}		
			}
		}
	}
	return 0;
}

void Read(ClientClass* client){
	while(!client->exit){
		char receivedMessage[1024] = {0};
		read( client->GetId() , receivedMessage, sizeof(receivedMessage));	
		std::string message(receivedMessage);
		if(message=="exit"){
			send(client->GetId(),message.c_str(),strlen(message.c_str()),0);
			client->exit=true;
			send(client->GetFriend()->GetId(), message.c_str(),strlen(message.c_str()),0);
			client->GetFriend()->exit=true;
		}
		else{
			if(message!=""){	
				std::string toSend = "\033[34m[" + client->GetName() + "] > \033[0m" + message;;
				std::cout<<toSend<<std::endl;
				send(client->GetFriend()->GetId(), toSend.c_str(),strlen(toSend.c_str()),0);	
			}
		}	
	}
}
