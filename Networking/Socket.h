#pragma once
#include "Address.h"

namespace Networking {

	class Socket
	{
	public:		
		NETWORKING_API Socket();
		NETWORKING_API ~Socket();
		NETWORKING_API bool Open(unsigned short port);
		NETWORKING_API void Close() const;
		NETWORKING_API bool IsOpen() const;
		NETWORKING_API bool Send(const Address & dest, char * data, const int data_size) const;
		NETWORKING_API bool Receive(Address & sender, char * data) const;

	private:
		int handle_;
	};
}


