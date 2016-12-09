#pragma once
#include "Socket.h"
#include "Server.h"

namespace Networking {
	class Client
	{
	public:
		NETWORKING_API Client(unsigned short port);
		NETWORKING_API bool ConnectTo(Address server);
		NETWORKING_API ~Client();
	private:
		Socket * socket_;
		bool connected_;
		unsigned short port_;
	};
}


