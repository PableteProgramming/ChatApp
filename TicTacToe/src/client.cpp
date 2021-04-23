#include <main.hpp>

void ClientRead(int sock,bool* running){
	while((*running)){
		char receivedMessage[1024]={0};
		read(sock,receivedMessage,sizeof(receivedMessage));
		if((*running)){	
			std::cout<<receivedMessage<<std::endl;
		}
	}
}


int main(int argc, char** argv)
{
	std::string ipAddress;
	std::string name;
	if (argc < 3)
	{
		std::cout << "Please pass a parameter" << std::endl;
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

	char receivedMessage[1024] = {0};
	read( sock , receivedMessage, sizeof(receivedMessage));	
	std::string startMessage(receivedMessage);

	std::cout << startMessage << std::endl;
	send(sock, name.c_str(), strlen(name.c_str()), 0);
	char CUsernameExists[1024]={0};
	read(sock,CUsernameExists,sizeof(CUsernameExists));
	std::string UserNameExists(CUsernameExists);
	if(UserNameExists=="Y"){
		std::cout<<"Username "<<name<<" already exists !"<<std::endl;
		return 1;
	}
	else{
		std::cout<<"valid Username"<<std::endl;
	}
	if(argc == 4){
		send(sock,"N",strlen("N"),0);
		char temp[1024]={0};
		read(sock, temp, sizeof(temp));
		send(sock,argv[3],strlen(argv[3]),0);
		char userExists[1024] = {0};
		read( sock , userExists, sizeof(userExists));
		std::string exists(userExists);
		if(exists=="N"){
			std::cout<<"Username "<<argv[3]<<" doesn't exists !"<<std::endl;
			std::cout<<"Try again!"<<std::endl;
			return 1;
		}
	}
	else{
		send(sock,"Y",strlen("Y"),0);
		std::cout<<"In the Waiting room..."<<std::endl;
		char temp[1024] = {0};
		read( sock , temp, sizeof(temp));	
	}

	bool running=true;
	std::thread reading(ClientRead,sock,&running);	
	while (running)
	{
		std::cout<<"\033[31m[You] > \033[0m";
		std::getline(std::cin, message);

		send(sock , message.c_str() , strlen(message.c_str()) , 0 );

		if (message == "exit")
			running = false;
			
	}
	reading.join();
	return 0;
}
