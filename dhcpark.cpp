#include "dhcpark.h"

int main(void)
{
	struct dhcp_packet Buffer;
	cout << sizeof(struct dhcp_packet) << endl;

	struct sockaddr_in serverInfo = {};
	struct sockaddr_in clientInfo = {};
	int recvLength = 0;

	socklen_t clientAddressSize = 0;

	serverInfo.sin_family = AF_INET; // use IPv4
	serverInfo.sin_addr.s_addr = INADDR_ANY; // htonl : change type long's byte sequence, host to network
	serverInfo.sin_port = htons(PORT_DHCP_CLIENT); // htons : change type short's byte sequence, host to network
	// SOCKET listenerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // tcp 
	//
	int cerverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // udp
	int option = 1;
	setsockopt(cerverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); // set multiple access to single port possible
	if(cerverSocket == -1)
	{
		cerr << "Cannot Create Server Socket, Quit." << endl;
		return -1;
	}

	if(bind(cerverSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) < 0)
	{
		cerr << "Cannot Bind Local Address, Quit." << endl;
		close(cerverSocket);
		return -1;
	}

	while(1)
	{
		printf("aaa\n");
		// wait to recv, save info of sender client
		clientAddressSize = sizeof(clientInfo);
		recvLength = recvfrom(cerverSocket, (char *)&Buffer, sizeof(Buffer), 0, \
				(struct sockaddr *)&clientInfo, &clientAddressSize);

		continue;
		if(recvLength > 0)
		{
<<<<<<< HEAD
//			printf("Receive byte of %lu, from %lu:%u\n", recvLength, clientInfo.sin_addr.s_addr, ntohs(clientInfo.sin_port));
=======
			printf("Receive byte of %lu, from %lu:%u\n", recvLength, inet_ntoa(clientInfo.sin_addr) ,ntohs(clientInfo.sin_port));
>>>>>>> 09b0e4554ba7445da4985d5c4a443ab4cfd3c54f
			cout << "DHCP MESSAGE PACKET : "<< endl;
			printf("op : %u\n", Buffer.op);
			printf("Hardware type = %u\n", Buffer.hw_type);
			printf("hopcount : %u\n", Buffer.hop_count);
			printf("transaction id : %u\n", Buffer.tx_id);
<<<<<<< HEAD
			printf("nsecs : %x\n", Buffer.nsecs);
=======
			printf("nsecs : %u\n", Buffer.nsecs);
>>>>>>> 09b0e4554ba7445da4985d5c4a443ab4cfd3c54f
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
	close(cerverSocket);
}


