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
		for(int i=pending.size()-1; i>=0;i--){
			std::cout<<"client "<<clients[pending[i]]->GetName()<<" exits!"<<std::endl;
			clients.erase(clients.begin()+pending[i]);
			std::cout<<clients.size()<<std::endl;
		}
	}
}

int main(){
#ifdef __linux__
	int sock, client;
	struct sockaddr_in socketObj;
	int opt = 1;
	int socketObjSize = sizeof(socketObj);
	std::string message = "Hello from server";

	// Creating socket file descriptor
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	socketObj.sin_family = AF_INET;
	socketObj.sin_addr.s_addr = INADDR_ANY;
	socketObj.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(sock, (struct sockaddr *)&socketObj, 
				 sizeof(socketObj))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(sock, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
#else
	#define WIN32_LEAN_AND_MEAN
	#pragma comment (lib, "Ws2_32.lib")
	
	WSADATA wsaData;
    int iResult;

    SOCKET sock = INVALID_SOCKET;
    SOCKET client = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, std::to_string(PORT).c_str(), &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( sock, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(sock, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
#endif

	bool running = true;
	std::thread exitclients(ExitClients);
	while (running)
	{
#ifdef __linux__
		client = accept(sock, (struct sockaddr *)&socketObj, (socklen_t*)&socketObjSize);
		if (client < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
#else
		client = accept(sock, NULL, NULL);
		if (client == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return 1;
		}
#endif
		else
		{
			std::string message = "\033[32mWelcome to the server!\033[0m";
			SocketSend(client, message);
			std::string nameMessage= SocketRead(client);
			bool UsernameExists=false;
			for(int i=0; i<clients.size();i++){
				if(clients[i]->GetName()==nameMessage){
					UsernameExists=true;
					break;
				}
			}

			if(UsernameExists){
				SocketSend(client,"Y");				
			}
			else{
				SocketSend(client,"N");
				std::string isWaiting= SocketRead(client);
				if(isWaiting=="Y"){
					waitingroom.push_back(WaitingClient(client,nameMessage));
				}
				else{
					SocketSend(client,"T");
					std::cout<<"Waiting friend name"<<std::endl;
					std::cout<<"Lets see what appends..."<<std::endl;
					std::string sFriendName= SocketRead(client);
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
						SocketSend(client,"Y");
						std::cout<<"Client "<<nameMessage<<" accepted"<<std::endl;
						std::cout<<"Client "<<waitingroom[index].GetName()<<" is now active"<<std::endl;

						ClientClass* temp= new ClientClass(waitingroom[index].GetId(),waitingroom[index].GetName());
						ClientClass* newclient= new ClientClass(client,nameMessage);
						SocketSend(temp->GetId(),"T");
						clients.push_back(temp);
						clients.push_back(newclient);
						temp->StartThread(newclient);
						newclient->StartThread(temp);
						waitingroom.erase(waitingroom.begin()+index);
					}
					else{
						SocketSend(client,"N");
					}
				}		
			}
		}
	}
	return 0;
}

void Read(ClientClass* client){
	while(!client->exit){
		std::string message= SocketRead(client->GetId());
		if(message=="exit"){
			SocketSend(client->GetId(),message);
			client->exit=true;
			SocketSend(client->GetFriend()->GetId(), message);
			client->GetFriend()->exit=true;
		}
		else{
			if(message!=""){	
				std::string toSend = "\033[34m[" + client->GetName() + "] > \033[0m" + message;;
				std::cout<<toSend<<std::endl;
				SocketSend(client->GetFriend()->GetId(), toSend);
			}
		}	
	}
}
