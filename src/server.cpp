#include <main.hpp>
std::vector<int> clients;
void Listen()
{
	//std::cout << "Listening ..." << std::endl;
	while(true){
		std::vector<int> pendingToExit;
		for(int i=0; i< clients.size();i++){
			char receivedMessage[1024] = {0};
			read( clients[i] , receivedMessage, sizeof(receivedMessage));
			
			std::string message(receivedMessage);
			if(message=="exit"){
				pendingToExit.push_back(i);
			}
			else{
				if(message!=""){	
					std::cout<<message<<std::endl;
				}
			}
		}
		for(int i=0; i< pendingToExit.size();i++){
			clients.erase(clients.begin()+pendingToExit[i]);
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
	std::thread listening(Listen);
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
			clients.push_back(client);
		}

		std::cout << clients.size() << std::endl;

		/*send(client , message.c_str() , strlen(message.c_str()) , 0 );
		printf("Hello message sent\n");*/
	}
	return 0;
}
