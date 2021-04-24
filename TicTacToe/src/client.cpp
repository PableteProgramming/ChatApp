#include <main.hpp>

#ifdef __linux__
	void ClientRead(int sock,bool* running)
#else
	void ClientRead(SOCKET sock,bool* running)
#endif
{
	while((*running)){
		std::string message = SocketRead(sock);
		if(message=="exit"){
			(*running)=false;
			break;
		}
		if((*running)){	
			std::cout<<message<<std::endl;
		}
	}
}

int main(int argc, char** argv)
{
	std::string ipAddress;
	std::string name;
	if (argc < 3)
	{
		std::cout << "Please pass a parameter!" << std::endl;
		return 0;
	}
	else
	{
		ipAddress = argv[1];
		name = argv[2];
	}

	int sock = 0, valread;
	struct sockaddr_in server_info;
	std::string message;
	char messageBuffer[1024] = {0};
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, ipAddress.c_str(), &server_info.sin_addr)<=0) 
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&server_info, sizeof(server_info)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	std::string startMessage = SocketRead(sock);

	std::cout << startMessage << std::endl;
	SocketSend(sock, name);
	std::string UserNameExists = SocketRead(sock);
	if(UserNameExists=="Y"){
		std::cout<<"Username "<<name<<" already exists !"<<std::endl;
		return 1;
	}
	else{
		std::cout<<"valid Username"<<std::endl;
	}
	if(argc == 4){
		SocketSend(sock, "N");
		std::string temp = SocketRead(sock);
		SocketSend(sock, std::string(argv[3]));
		std::string exists = SocketRead(sock);
		if(exists=="N"){
			std::cout<<"Username "<<argv[3]<<" doesn't exists !"<<std::endl;
			std::cout<<"Try again!"<<std::endl;
			return 1;
		}
	}
	else{
		SocketSend(sock,"Y");
		std::cout<<"In the Waiting room..."<<std::endl;
		std::string temp = SocketRead(sock);	
	}

	bool running=true;
	std::thread reading(ClientRead,sock,&running);	
	while (running)
	{
		std::cout<<"\033[31m[You] > \033[0m";
		std::getline(std::cin, message);

		SocketSend(sock, message);

		if (message == "exit")
			running = false;
			
	}
	reading.join();
	return 0;
}
