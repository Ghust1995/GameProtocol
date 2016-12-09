#include "Address.h"


namespace Networking {
	Address::Address()
	{
		address_ = 0;
		port_ = 0;
	}
	Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
	{
		address_ = (a << 24) |
			(b << 16) |
			(c << 8) |
			d;
		port_ = port;
	}

	Address::Address(unsigned int address, unsigned short port)
	{
		address_ = address;
		port_ = port;
	}

	Address::~Address()
	{
	}
	
	unsigned int Address::GetAddress() const
	{
		return address_;
	}

	unsigned short Address::GetPort() const
	{
		return port_;
	}
}

