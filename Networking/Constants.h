#pragma once

namespace Networking {
	namespace  Constants {
		const unsigned int PROTOCOL_ID = 0x7074756c;
		const int PROTOCOL_HEADER_SIZE = 4*sizeof(unsigned int);
		const int PACKET_SIZE = 256;
		const double TIMEOUT = 10;
		const double SERVER_MESSAGE_FREQUENCY = 2.0;
		const double CLIENT_MESSAGE_FREQUENCY = 1;
		const bool SIMULATE_PACKAGE_LOSS = true;
		const int PACKAGE_LOSS_PROBABILITY = 15;
	}
}