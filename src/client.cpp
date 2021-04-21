#include <main.hpp>

int main(int argc, char** argv)
{
	std::string ipAddress;
	if (argc < 2)
	{
		std::cout << "Please pass a parameter" << std::endl;
		return 0;
	}
	else
	{
		ipAddress = argv[1];
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
	
	bool running = true;
	while (running)
	{
		std::cout<<"\033[31m[input] > \033[0m";
		std::getline(std::cin, message);

		send(sock , message.c_str() , strlen(message.c_str()) , 0 );

		if (message == "exit")
			running = false;
			
	}
	return 0;
}
