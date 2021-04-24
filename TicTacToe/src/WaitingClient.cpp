#include <WaitingClient.hpp>

#ifdef __linux__
	WaitingClient::WaitingClient(int _id,std::string _name)
#else
	WaitingClient::WaitingClient(SOCKET _id,std::string _name)
#endif
{
	id=_id;
	name=_name;
}