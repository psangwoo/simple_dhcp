#include "dhcpark.h"

struct dhcp_packet build_packet(struct dhcp_packet received, allocInfo info)
{

	struct dhcp_packet ret;

	vector<unsigned char> opt;
	memset(&ret, 0x00, sizeof(ret));

	memcpy(&ret, &received, 236);

	memset(&ret.op, 0x02, sizeof(ret.op));

	int req = 0;
	int leaset = 0;
	int digit = 16777216;

	opt.push_back(0x63);
	opt.push_back(0x82);
	opt.push_back(0x53);
	opt.push_back(0x63);
	int i = 4;
	while(received.options[i] != 0xff)
	{
		int code = received.options[i++];
		int len = received.options[i++];

		switch(code)
		{
			case 12: // host name : no use
				break;
			case 50: // Requested IP : check if this ip is empty, return this ip if can otherwise available ip address to YIPADDR area
				req = received.options[i + 3];
				break;
			case 53: // Opcode : Include accordin t o this value
				opt.push_back(53);
				opt.push_back(1);
				if(received.options[i] == 1) // if discover, reply offer
					opt.push_back(2);
				else if(received.options[i] == 3) // if request, reply ack
				{
					opt.push_back(5);
					
					opt.push_back(54); // send server info
					opt.push_back(4);

					for(unsigned char uc : info.conf.serveraddr)
						opt.push_back(uc);
					opt.push_back(51); // send lease time
					opt.push_back(4);
					for(unsigned char uc : info.conf.leasetime)
					{
						opt.push_back(uc);
						leaset += (int )uc * digit;
						printf("leaset : %d uc = %d digit : %d\n", leaset, (int)uc, digit);
						digit /= 256;
					}
				}
				break;
			case 54: // Server ID - check this with my ip , revert if not equal
				break;
			case 55: // Parameter Request List
				for(int k = 0; k < len; k++)
				{
					switch(received.options[i + k])
					{
						case 1: // Subnet Mask
							opt.push_back(1);
							opt.push_back(4);
							for(unsigned char uc : info.conf.netmask)
								opt.push_back(uc);
							break;
						case 2: // Time-Zone
							break;
						case 3: // Default Gateway Address
							opt.push_back(3);
							opt.push_back(4);
							for(unsigned char uc : info.conf.gateway)
								opt.push_back(uc);
							break;
						case 6: // DNS Address
							opt.push_back(6);
							opt.push_back(4);
							for(unsigned char uc : info.conf.broadcast)
								opt.push_back(uc);
							break;
						case 12: // Host Name
							break;
						case 15: // Domain Name
							opt.push_back(15);
							opt.push_back(info.conf.domain.length());
							for(unsigned char uc : info.conf.domain)
								opt.push_back(uc);
							break;
						case 26: //  MTU
							break;
						case 28: // Broadcast Address
							opt.push_back(28);
							opt.push_back(4);
							for(unsigned char uc : info.conf.broadcast)
								opt.push_back(uc);
							break;
						case 31: // Router Discovery
							break;
						case 33: // Static Route
							break;
						case 40: // YD
							break;
						case 41: // YS
							break;
						case 42: // NTP
							break;
						case 43: // Vendor Option
							break;
						case 44: // NetBios Name Server
							break;
						case 46: // NetBios Node
							break;
						case 47: // Netbios Scope
							break;
						case 119: // Domain Search
							break;
						case 121: // Classless Static Route
							break;
						case 249: // Classless Static Route - Microsoft
							break;
					}
				}
				break;
			case 60: // vendor-class option, what to do
				break;
			case 61: // client ID option  : use for what ?
				break;

		}
		i += len;

	}

	req = info.addrtoalloc(req, info.conf.maxrange);

	
	string ipaddr;
	ipaddr = info.conf.subnet;
	cout << ipaddr << endl;
	ipaddr += to_string(req);


	cout << ipaddr << endl;

	inet_aton(ipaddr.c_str(), &ret.yipaddr);

	info.noticealloc(req, leaset);

	opt.push_back(255);


	memcpy(ret.options, &opt.front(), opt.size());
	return ret;

}


void *dhcp(void *tmp)
{
	struct dhcp_packet Buffer;

	allocInfo *info = (allocInfo *)tmp;

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
	struct sockaddr_in sendInfo = {};

	sendInfo.sin_family = AF_INET;
	sendInfo.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sendInfo.sin_port = htons(PORT_DHCP_CLIENT);
	int sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sendSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	setsockopt(sendSocket, SOL_SOCKET, SO_BROADCAST, &option, sizeof(option)); 	

	if(serverSocket == -1)
	{
		cerr << "Cannot Create Server Socket, Quit." << endl;
		return (void *)-1;
	}

	if(bind(serverSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) < 0)
	{
		cerr << "Cannot Bind Client Local Address, Quit." << endl;
		close(serverSocket);
		return (void *)-1;
	}

	if(sendSocket == -1)
	{
		cerr << "Cannot Create Send Socket, Quit." << endl;
		return (void *)-1;
	}

	if(bind(sendSocket, (struct sockaddr *)&sendInfo, sizeof(sendInfo)) < 0)
	{
		cerr << "Cannot Bind Sender Local Address, Quit." << endl;
		close(serverSocket);
		return (void *)-1;
	}

	int sendsize = 0;
	

	while(1)
	{
		cout << "wait input " << endl;
		// wait to recv, save info of sender client
		clientAddressSize = sizeof(clientInfo);
		recvLength = recvfrom(serverSocket, (char *)&Buffer, sizeof(Buffer), 0, \
				(struct sockaddr *)&clientInfo, &clientAddressSize);
		if(recvLength > 0)
		{
			
			struct dhcp_packet sendBuf = build_packet(Buffer, *info);

			sendsize = sendto(sendSocket, (char *)&sendBuf, sizeof(sendBuf), 0, \
					(struct sockaddr *)&sendInfo, sizeof(sendInfo));
			if(sendsize < 0)
			{
				cerr << "Couldn't Reply, Quit.\n" <<endl;
				return (void *)-1;
			}


		}
	}
	close(serverSocket);
}


