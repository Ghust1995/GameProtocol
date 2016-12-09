// NetworkingClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Socket.h"
#include "Address.h"
#include <iostream>
#include "Server.h"
#include "Client.h"
#include "Constants.h"

using namespace Networking;
using namespace std;

unsigned const short CLIENT_PORT = 6001;
unsigned const short SERVER_PORT = 5001;

int main()
{
	char yn;
	printf("Client? (y/n) ");
	cin.get(yn);

	bool isSender = yn == 'y' || yn == 'Y' ? true : false;
	

	if(isSender) {
		unsigned short client_port;
		unsigned short server_port;
		//printf("Enter a port to run client: ");
		//cin >> port;
		client_port = CLIENT_PORT;
		auto client = new Client(client_port);		
		
		//printf("Enter a port to send to: ");
		//cin >> port;
		server_port = SERVER_PORT;
		const Address server(127, 0, 0, 1, server_port);

		client->ConnectTo(server);				
	} else {
		unsigned short port;
		//printf("Enter a port to run server: ");
		//cin >> port;
		port = SERVER_PORT;
		auto server = new Server(port);
		server->Start();
	}

	return 0;
}

