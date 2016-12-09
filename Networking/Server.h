#pragma once
#include "Socket.h"

namespace Networking {
	class Server
	{
	public:
		NETWORKING_API Server(unsigned short port);
		NETWORKING_API bool Start();
		NETWORKING_API ~Server();
	private: 
		Socket * socket_;		
		bool connected_;
		unsigned short port_;
	};
}


