#include <main.hpp>
std::vector<int> clients;
std::vector<std::thread> threads;
void Read(int i){
	while(true){
		char receivedMessage[1024] = {0};
		read( clients[i] , receivedMessage, sizeof(receivedMessage));	
		std::string message(receivedMessage);
		if(message=="exit"){
			clients.erase(clients.begin()+i);
			threads.erase(threads.begin()+i);
			break;
		}
		else{
			if(message!=""){	
				std::cout<<message<<std::endl;
			}
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
			threads.push_back(std::thread(Read,clients.size()-1));
		}


	}
	return 0;
}
