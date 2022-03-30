#include "dhcpark.h"

/*


void handle_option(int option, int len, struct dhcp_packet *data)
{
	switch(option)
	{
		case 1:
			//subnet mask
		case 3:

		case 55:
			// 
			break;


	}
}



void packet_send(int *options, int op_len, int request_ip)
{
	struct dhcp_packet packet;
	packet.op = 2;

	for(int i = 0; i < op_len; i++)
	{
		switch(options[i])
		{

			case 55:

		}
	}
}
*/

int main(void)
{
	struct dhcp_packet Buffer;
	cout << sizeof(struct dhcp_packet) << endl;
	ifstream configFile;
	ofstream leaseLogFile;

	struct sockaddr_in serverInfo = {};
	struct sockaddr_in clientInfo = {};
	int recvLength = 0;

	socklen_t clientAddressSize = 0;

	serverInfo.sin_family = AF_INET; // use IPv4
	serverInfo.sin_addr.s_addr = htonl(INADDR_ANY); // htonl : change type long's byte sequence, host to network
	serverInfo.sin_port = htons(PORT_DHCP_SERVER); // htons : change type short's byte sequence, host to network
	// SOCKET listenerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // tcp 
	//
	int serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // udp
	int option = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); // set multiple access to single port possible
	if(serverSocket == -1)
	{
		cerr << "Cannot Create Server Socket, Quit." << endl;
		return -1;
	}

	if(bind(serverSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) < 0)
	{
		cerr << "Cannot Bind Local Address, Quit." << endl;
		close(serverSocket);
		return -1;
	}

	while(1)
	{
		// wait to recv, save info of sender client
		clientAddressSize = sizeof(clientInfo);
		recvLength = recvfrom(serverSocket, (char *)&Buffer, sizeof(Buffer), 0, \
				(struct sockaddr *)&clientInfo, &clientAddressSize);
		if(recvLength > 0)
		{
			printf("Receive byte of %lu, from %lu:%u\n", recvLength, inet_ntoa(clientInfo.sin_addr) ,ntohs(clientInfo.sin_port));
			cout << "DHCP MESSAGE PACKET : "<< endl;
			printf("op : %u\n", Buffer.op);
			printf("Hardware type = %u\n", Buffer.hw_type);
			printf("hopcount : %u\n", Buffer.hop_count);
			printf("transaction id : %u\n", Buffer.tx_id);
			printf("nsecs : %u\n", Buffer.nsecs);
			printf("cipaddr : %s\n", inet_ntoa(Buffer.cipaddr));
			printf("yipaddr : %s\n", inet_ntoa(Buffer.yipaddr));
			printf("sipaddr : %s\n", inet_ntoa(Buffer.sipaddr));
			printf("gipaddr : %s\n", inet_ntoa(Buffer.gipaddr));
			for(int i = 0 ; i < 6; i++)
				printf("%x:", Buffer.chwaddr[i]);
			cout << endl;
			printf("Server name : %s\n", Buffer.sname);
			printf("Boot File path : %s\n", Buffer.bfile);
			printf("Options : \n");
			if(!(Buffer.options[0] == 0x63 && Buffer.options[1] == 0x82 && Buffer.options[2] == 0x53 \
						&& Buffer.options[3] == 0x63))
			{
				cerr << "DHCP Option Field Magic Cookie not right, Quit. " << endl;
				return -1;
			}
			int i = 4;
			while(Buffer.options[i] != 0xff)
			{
				printf("Option : %d ", Buffer.options[i++]);
				/*
				switch(Buffer.options[i])
				{
					case 50: // preferred IP address reqeust
						break;
					case 53: // check message type : 1 : discover, 3 : request, ...
						break;
					case 55: // PRL : parameter request list, send packets
						break; // save option datas to buff ? may be ? 

				}
				*/
				printf("Option length = %d, Data ", Buffer.options[i]);
				int tmp = Buffer.options[i++];
				for(int k = 0; k < tmp; k++)
				{
					printf(" %d ", Buffer.options[i + k]);
				}
				i = i + tmp;
				cout << endl;
			}
			printf("END : %d\n", Buffer.options[i]);
		}
	}
	close(serverSocket);
}


