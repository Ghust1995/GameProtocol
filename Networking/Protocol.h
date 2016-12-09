#pragma once
namespace Networking {
	struct Protocol {
		unsigned int protocol_id;
		unsigned int sequence;
		unsigned int ack;
		unsigned int ack_bitfield;
		char * data;

		static Protocol Parse(char * str);

		static char * GeneratePacket(char * msg_content, unsigned int sequence, unsigned int ack, unsigned int ack_bitfield);
	};

	
}



