#include <main.hpp>
#include <client.hpp>
#include <SFML/Graphics.hpp>

std::thread *GameThread=NULL;
bool running=true;
bool waitingroom=true;

#ifdef __linux__
	void ClientRead(int sock,bool* running)
#else
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")
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

#ifdef __linux__
	int sock = 0;
	struct sockaddr_in server_info;

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

#else
	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	struct addrinfo *result= NULL;
	struct addrinfo *ptr= NULL;
	struct addrinfo hints;
	int iResult= WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult!=0){
		printf("Error when init WSAStartup!\n");
		exit(1);
	}
	ZeroMemory(&hints,sizeof(hints));
	hints.ai_family= AF_UNSPEC;
	hints.ai_socktype= SOCK_STREAM;
	hints.ai_protocol= IPPROTO_TCP;

	iResult = getaddrinfo(argv[1], std::to_string(PORT).c_str(), &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        sock = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

	freeaddrinfo(result);

    if (sock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

#endif

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
		running=true;
		waitingroom=false;
		GameThread= new std::thread(RunWindow,&waitingroom,&running);
	}
	else{
		SocketSend(sock,"Y");
		std::cout<<"In the Waiting room..."<<std::endl;
		running=true;
		waitingroom=true;
		GameThread= new std::thread(RunWindow,&waitingroom,&running);
		SocketRead(sock);
		waitingroom=false;	
	}

	std::thread reading(ClientRead,sock,&running);
	Sleep(4000);
	SocketSend(sock,"exit");
	running=false;
	reading.join();
	GameThread->join();
	return 0;
}

void RunWindow(bool* waitingroom, bool* running){
	sf::RenderWindow window(sf::VideoMode(550,700), "Flappy Bird", sf::Style::Titlebar | sf::Style::Close);
	sf::Texture w;
	w.loadFromFile("wr.jpg");
	sf::Sprite ws(w);
	sf::Texture g;
	g.loadFromFile("t.png");
	sf::Sprite gs(g);
	while ((*waitingroom))
	{
		sf::Event event;
       	while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

	 	window.clear();
		//display message
		window.draw(ws);
	 	window.display();
	}

	while ((*running))
	{
		sf::Event event;
       	while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

	 	window.clear();
		//do game
		window.draw(gs);
	 	window.display();
	}
	window.close();
}