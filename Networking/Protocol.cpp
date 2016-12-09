#include "Protocol.h"
#include "Constants.h"
#include <string.h>
#include <cstdlib>

namespace  Networking {
	Protocol Networking::Protocol::Parse(char * str) {
		Protocol p;
		char * packet_data = str;

		p.protocol_id = *(unsigned int *)packet_data;
		packet_data += sizeof(unsigned int);

		p.sequence = *(unsigned int *)packet_data;
		packet_data += sizeof(unsigned int);

		p.ack = *(unsigned int *)packet_data;
		packet_data += sizeof(unsigned int);

		p.ack_bitfield = *(unsigned int *)packet_data;
		packet_data += sizeof(unsigned int);

		p.data = packet_data;
		return p;
	}

	char * Protocol::GeneratePacket(char * msg_content, unsigned int sequence, unsigned int ack, unsigned int ack_bitfield) {
		auto packet_data = (char*)malloc(Constants::PACKET_SIZE);
		auto data_start = packet_data;
		auto msg_header = (unsigned int *)packet_data;
		
		msg_header[0] = Constants::PROTOCOL_ID;
		msg_header[1] = sequence;
		msg_header[2] = ack;
		msg_header[3] = ack_bitfield;

		packet_data += Constants::PROTOCOL_HEADER_SIZE;
		strcpy_s(packet_data, Constants::PACKET_SIZE - Constants::PROTOCOL_HEADER_SIZE, msg_content);
		packet_data -= Constants::PROTOCOL_HEADER_SIZE;

		return packet_data;
	}
}

