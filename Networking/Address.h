#pragma once

#ifdef NETWORKING_EXPORTS
#define NETWORKING_API __declspec(dllexport) 
#else
#define NETWORKING_API __declspec(dllimport) 
#endif

namespace Networking {
	class Address
	{
	public:
		NETWORKING_API Address();
		NETWORKING_API Address(unsigned char a,
				unsigned char b,
				unsigned char c, 
				unsigned char d, 
				unsigned short port);
		NETWORKING_API Address(unsigned int address,
				unsigned short port);
		NETWORKING_API ~Address();
		NETWORKING_API unsigned int GetAddress() const;
		NETWORKING_API unsigned short GetPort() const;

	private:

		unsigned int address_;
		unsigned short port_;
	};
}


