#include <main.hpp>

int main()
{
	int sock = 0, valread;
	struct sockaddr_in server_info;
	std::string message = "Hello from client";
	char messageBuffer[1024] = {0};
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &server_info.sin_addr)<=0) 
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&server_info, sizeof(server_info)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	
	send(sock , message.c_str() , strlen(message.c_str()) , 0 );
	printf("Hello message sent\n");
	valread = read( sock , messageBuffer, 1024);
	printf("%s\n",messageBuffer );
	return 0;
}
