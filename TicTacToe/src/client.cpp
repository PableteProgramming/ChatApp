#include <main.hpp>
#include <client.hpp>

std::thread *GameThread=NULL;
bool running=true;
bool waitingroom=true;
bool turn=false;
bool needDraw = true;
bool exiting=true;
int win=404;

#ifdef __linux__
int sock;
void ClientRead(Player* player)
#else
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
SOCKET sock;
void ClientRead(Player* player)
#endif
{
	while(running){
		//std::cout << "Waiting message" << std::endl;
		std::string message = SocketRead(sock);
		if(message=="exit"){
			std::cout<<"Connection closed..."<<std::endl;
			exiting=false;
			running=false;
			break;
		}
		else{
			turn=true;
			// Parsing message
			char sx = message[0];
			char sy = message[1];
			int x = std::atoi(std::string(1, sx).c_str());
			int y = std::atoi(std::string(1, sy).c_str());
			//std::cout << x << y << std::endl;
			player->SetPos(x, y,player->GetOppositeSign());
			int temp = player->Win();
			if (temp >= -1 && temp <= 1)
			{
				//running = false;
				//std::cout << temp << std::endl;
				win=temp;
				running=false;
			}
			needDraw = true;
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
	sock = 0;
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
	sock = INVALID_SOCKET;
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
	signal(SIGBREAK, Client_Terminate_Handler);
#endif
	signal(SIGINT, Client_Terminate_Handler);

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
		turn=false;
		running=true;
		waitingroom=false;
		GameThread= new std::thread(RunWindow,sock);
	}
	else{
		SocketSend(sock,"Y");
		turn=true;
		std::cout<<"In the Waiting room..."<<std::endl;
		running=true;
		waitingroom=true;
		GameThread= new std::thread(RunWindow,sock);
		std::string response= SocketRead(sock);
		if (response=="exit"){
			std::cout<<"connection closed"<<std::endl;
			exit(0);
		}
		waitingroom=false;
		SocketSend(sock,"exit");
	}

	//while(running){}
	GameThread->join();
	//running=false;
	return 0;
}

#ifdef __linux__
void RunWindow(int sock){
#else
void RunWindow(SOCKET sock){
#endif
	exiting=true;
	char sign;

	if (waitingroom)
	{
		sign = 'x';
	}
	else
	{
		sign = 'o';
	}

	Player player(sign, turn, scale);
	std::thread* reading = NULL;
	sf::RenderWindow window(sf::VideoMode(scale * 3,scale * 3), "TicTacToe", sf::Style::Titlebar | sf::Style::Close);
	sf::Texture w;
	w.loadFromFile("wr.jpg");
	sf::Sprite ws(w);
	sf::Texture g;
	g.loadFromFile("t.png");
	sf::Sprite gs(g);

	while (waitingroom)
	{
		sf::Event event;
       	while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				window.close();
				exiting=false;
				running=false;
				waitingroom=false;
				//break;
			}
		}

	 	window.clear();
		//display message
		window.draw(ws);
	 	window.display();
	}

	if (running)
	{
		//std::cout << "Starting reading thread" << std::endl;
		reading = new std::thread(ClientRead,&player);
	}
	
	if(turn){
		window.setTitle("its your turn");
	}
	else{
		window.setTitle("its not your turn");
	}

	while (running)
	{
		sf::Event event;
	    while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				window.close();
				exiting=false;
				running=false;
			}
		}

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			if(turn){
				window.setTitle("its your turn");
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				
				int wx = mPos.x;
				int wy = mPos.y;

				int x  = wx / scale;
				int y = wy / scale;

				if (player.PosOK(x, y))
				{
					turn = false;
					std::string pos = std::to_string(x) + std::to_string(y);
					//window.setTitle(pos);
					//std::cout<<pos<<std::endl;
					SocketSend(sock,pos);
					player.SetPos(x,y,player.GetSign());

					int temp = player.Win();
					if (temp >= -1 && temp <= 1)
					{
						//running = false;
						//std::cout << temp << std::endl;
						win=temp;
						running=false;
					}
					needDraw = true;
				}
			}
			else{
				window.setTitle("its not your turn");
			}
		}

		if (needDraw)
		{
			window.clear();	
			player.DrawGrid(window);
			window.display();
			needDraw = false;
		}
	}

	//std::cout << "Sending exit" << std::endl;
	SocketSend(sock,"exit");

	if (reading != NULL)
	{
		//std::cout << "Joining reading" << std::endl;
		reading->join();
	}
	else
	{
		//std::cout << "EXIT" << std::endl;
		exit(0);
	}

	/*if(!exiting){
		exit(0);
	}*/

	switch (win)
	{
		case -1:{
			//you lose
			window.setTitle("You lose");
			exiting=true;
			break;
		}
		case 0:{
			//draw
			exiting=true;
			window.setTitle("It's a draw");
			break;
		}
		case 1:{
			//you win
			exiting=true;
			window.setTitle("You win !");
			break;
		}
		default:{
			window.setTitle("I don't know what happend");
			break;
		}
	}

	//exiting=true;
	while (exiting){
		sf::Event event;
	    while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				//window.close();
				exiting=false;
			}
		}
		window.clear();
		window.display();
	}
	window.close();
}

void Client_Terminate_Handler(int sig) {
	//if (sig == 2) {
		//Ctrl C
	exiting=false;
	running = false;
	SocketSend(sock, "exit");
	//std::cout << "Server connection closed successfully" << std::endl;
	//}
}