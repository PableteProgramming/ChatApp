#include <WaitingClient.hpp>

#ifdef __linux__
	WaitingClient::WaitingClient(int _id,std::string _name)
#else
	WaitingClient::WaitingClient(SOCKET _id,std::string _name)
#endif
{
	id=_id;
	name=_name;
	reading = NULL;
	waiting = true;
}

void WaitingClient::StartThread()
{
	reading = new std::thread(WaitingClientRead, id, &waiting);
}

void WaitingClient::JoinThread()
{
	reading->join();
}

#ifdef __linux__
	void WaitingClientRead(int _socket, bool* waiting)
#else
	void WaitingClientRead(SOCKET _socket, bool* waiting)
#endif
	{
		std::cout << "Starting thread..." << std::endl;
		std::string message = "notexit";
        while(message!="exit"){
			std::cout << "Waiting message" << std::endl;
            message = SocketRead(_socket);
			std::cout << message << std::endl;
        }
		(*waiting) = false;
    }