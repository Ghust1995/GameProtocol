#include "Socket.h"

#include <stdio.h>
#include "Constants.h"


#pragma region Platform Socket Includes
// platform detection
#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <WinSock2.h>

#elif PLATFORM == PLATFORM_MAC || 
PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

#if PLATFORM == PLATFORM_WINDOWS
#pragma comment( lib, "wsock32.lib" )
#endif
#pragma endregion

namespace Networking {
	Socket::Socket()
	{
		handle_ = -1;
#if PLATFORM == PLATFORM_WINDOWS
		WSADATA WsaData;
		WSAStartup(MAKEWORD(2, 2),
			&WsaData)
			== NO_ERROR;
#endif
	}

	Socket::~Socket()
	{
#if PLATFORM == PLATFORM_WINDOWS
		WSACleanup();
#endif
	}

	bool Socket::Open(unsigned short port)
	{
		// Creates a udp socker
		handle_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (handle_ < 0) {
			printf("Failed to create socket\n");
			return false;
		}

		// Bind socket to port
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;

		// Converts to host byte order (little or big endian)
		address.sin_port = htons(static_cast<unsigned short>(port));

		if (bind(handle_,
			reinterpret_cast<const sockaddr*>(&address),
			sizeof(sockaddr_in)) < 0) {
			printf("Failed to bind socket\n");
			return false;
		}

		// Set socket to non-blocking mode (blocking doesnt make sense in real time)
#if PLATFORM == PLATFORM_WINDOWS
		DWORD nonBlocking = 1;
		if (ioctlsocket(handle_, FIONBIO, &nonBlocking) != 0) {
			printf("Failed to set non-blocking\n");
			return false;
		}
#elif PLATFORM == PLATFORM_MAC || 
		PLATFORM == PLATFORM_UNIX
			if (fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
				printf("Failed to set non-blocking\n");
				return false;
			}
#endif

		return true;
	}

	NETWORKING_API void Networking::Socket::Close() const {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
		close(handle_);
#elif PLATFORM == PLATFORM_WINDOWS
		closesocket(handle_);
#endif
	}

	bool Socket::Send(const Address & dest, char * data,  const int data_size) const {
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(dest.GetAddress());
		addr.sin_port = htons(dest.GetPort());

		int sent_bytes = sendto(handle_,
			data,
			data_size,
			0,
			reinterpret_cast<sockaddr *>(&addr),
			sizeof(sockaddr_in));

		if (sent_bytes != data_size) {
			printf("Failed to send packet\n");
			return false;
		}
		return true;
	}

	bool Socket::Receive(Address & sender, char * data) const {
		char packet_data[Constants::PACKET_SIZE];

		unsigned int max_packet_size = sizeof(packet_data);

#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
#endif
		sockaddr_in from;
		socklen_t fromLength = sizeof(from);

		socklen_t addrLength = sizeof(from);
		int bytes = recvfrom(handle_,
			packet_data,
			max_packet_size, 0,
			reinterpret_cast<sockaddr*>(&from),
			&addrLength);

		// No more packets to handle
		if (bytes <= 0) {
			return false;
		}	
		
		if (Constants::SIMULATE_PACKAGE_LOSS && rand() % 100 < Constants::PACKAGE_LOSS_PROBABILITY) {
			printf("### Package lost!\n");
			return false;
		}

		//data = malloc(bytes);
		memcpy_s(data, Constants::PACKET_SIZE, packet_data, Constants::PACKET_SIZE);
		sender = Address(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port));

		return true;
	}
}


