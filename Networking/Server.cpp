#include "Server.h"
#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <chrono>
#include "Constants.h"
#include "Protocol.h"

using namespace std::chrono;

namespace  Networking {
	NETWORKING_API Server::Server(unsigned short port) {
		port_ = port;
		socket_ = new Socket();
		connected_ = false;
	}

	NETWORKING_API bool Server::Start() {
		Address client;
		unsigned int local_sequence = 0;
		unsigned int remote_sequence = 0;
		int ack_bitfield = 0;
		
		if (!socket_->Open(port_)) {
			printf("Failed to start server");
			return false;
		}
		printf("Server running on port %d\n", port_);
		auto last_packet_sent_time = steady_clock::now();
		auto last_packet_received_time = steady_clock::now();
		while (true) {
			char data_buffer[256];
			Address sender;
			if (socket_->Receive(sender, data_buffer)) {
				auto parsed_msg = Protocol::Parse(data_buffer);
				
				// First message from a client
				if (parsed_msg.protocol_id == Constants::PROTOCOL_ID) {
					printf("<<< Message received:\t %s  | SEQ: %u  \t| ACK: %u  \t| ACK_BF: %u\n", parsed_msg.data, parsed_msg.sequence, parsed_msg.ack, parsed_msg.ack_bitfield);
					if (parsed_msg.sequence > remote_sequence) {
						int diff = parsed_msg.sequence - remote_sequence;
						ack_bitfield = (ack_bitfield & (~(~(1 << diff) << 31))) << diff;
						remote_sequence = parsed_msg.sequence;
					}

					// Set ack of received packet to 1
					if(remote_sequence - parsed_msg.sequence >= 0) {
						ack_bitfield |= (1 << (remote_sequence - parsed_msg.sequence));
					}					

					if (!connected_) {
						client = sender;
						connected_ = true;
						remote_sequence = 0;
						ack_bitfield = 0;
						printf("--- Connected to: %x:%d\n", client.GetAddress(), client.GetPort());
					}

					last_packet_received_time = steady_clock::now();
				}						
			}
			
			if(connected_) {
				// Constantly keep sending messages
				auto secondsSinceLastPacketSent = (steady_clock::now() - last_packet_sent_time).count() * steady_clock::period::num / steady_clock::period::den;
				if(secondsSinceLastPacketSent > Constants::SERVER_MESSAGE_FREQUENCY) {
					char msg_content[256 - 4] = "Hello client";
					
					auto msg_correct = Protocol::GeneratePacket(msg_content, local_sequence, remote_sequence, ack_bitfield);

					socket_->Send(client, msg_correct, 256);
					printf(">>> Message sent:\t %s  | SEQ: %u  \t| ACK: %u  \t| ACK_BF: %u\n", msg_content, local_sequence, remote_sequence, ack_bitfield);

					local_sequence++;
					last_packet_sent_time = steady_clock::now();
				}

				// Check for client timeout
				auto secondsSinceLastPacketReceived = (steady_clock::now() - last_packet_received_time).count() * steady_clock::period::num / steady_clock::period::den;
				if (secondsSinceLastPacketReceived > Constants::TIMEOUT) {
					printf("### Connection timeout, client disconnected\n");
					connected_ = false;
				}
			}			
		}
		return true;
	}

	NETWORKING_API Server::~Server(){
		socket_->Close();
	}
}

