#include "dhcpark.h"




struct dhcp_packet build_packet(struct dhcp_packet received, allocInfo info, int *sendsize)
{

	struct dhcp_packet ret;

	vector<unsigned char> opt;
	memset(&ret, 0x00, sizeof(ret));

	memcpy(&ret, &received, 236);

	memset(&ret.op, 0x02, sizeof(ret.op));
	bool alloced = false;
	bool released = false;
	int req = 0;
	int leaset = 0;
	int digit = 16777216;  // 256 ^ 3


	int clientpos = -1;
	if(strcmp(inet_ntoa(received.cipaddr), "0.0.0.0"))
	{
		clientpos = stoi(split(inet_ntoa(received.cipaddr), '.')[3]);
		req = clientpos;
	}


	unsigned char hostname[100];
	int hostnamelen =0;

	bool hostnameIn = false;

	/* Magic Cookie */
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
			case 12: // host name
				hostnamelen = len;
				for(int t = 0; t < len; t++)
					hostname[t] = received.options[i + t];
				break;
			case 50: // Requested IP : check if this ip is empty, return this ip if can otherwise available ip address to YIPADDR area
				req = received.options[i + 3];
				break;
			case 53: // Opcode : Include according to this value
				opt.push_back(53);
				opt.push_back(1);
				if(received.options[i] == DISCOVER) // if discover, reply offer
					opt.push_back(OFFER);
				else if(received.options[i] == REQUEST) // if request, reply ack
				{
					alloced = true;
					opt.push_back(ACK);
					
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
						digit /= 256;
					}
				}
				else if(received.options[i] == RELEASE || received.options[i] == 255) // if release, release and no send file
				{
					info.freealloc(clientpos);
					released = true;
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
							for(unsigned char uc : info.conf.dns)
								opt.push_back(uc);
							break;
						case 12: // Host Name
							hostnameIn = true;
							break;
						case 15: // Domain Name
							opt.push_back(15);
							opt.push_back(info.conf.domain.length());
							for(unsigned char uc : info.conf.domain)
								opt.push_back(uc);
							break;
						case 26: //  MTU
							opt.push_back(26);
							opt.push_back(2);
							opt.push_back(5);
							opt.push_back(220);

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

	req = info.addrtoalloc(req, info.conf.maxrange, clientpos, convert_tomac(received.chwaddr));

	
	string ipaddr;
	ipaddr = info.conf.subnet;
	ipaddr += to_string(req);

	if(hostnameIn)
	{
		opt.push_back(12);
		opt.push_back(hostnamelen);
		for(int k = 0; k < hostnamelen; k++)
			opt.push_back(hostname[k]);
	}

	inet_aton(ipaddr.c_str(), &ret.yipaddr);

	if(alloced)
		info.noticealloc(req, leaset, convert_tomac(received.chwaddr));

	opt.push_back(255);


	memcpy(ret.options, &opt.front(), opt.size());

	*sendsize = ((opt.size() + 236) / 32 + 1) * 32;
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
	serverInfo.sin_addr.s_addr = htons(INADDR_ANY); // htonl : change type long's byte sequence, host to network
	serverInfo.sin_port = htons(PORT_DHCP_SERVER); // htons : change type short's byte sequence, host to network
	
	int serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // udp
	int option = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); // set multiple access to single port possible
	setsockopt(serverSocket, SOL_SOCKET, SO_BROADCAST, &option, sizeof(option)); // set broadcast available

	struct sockaddr_in sendInfo = {};

	sendInfo.sin_family = AF_INET;
	sendInfo.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sendInfo.sin_port = htons(PORT_DHCP_CLIENT);

	char *nicName = findnic(info->conf.subnet);
	if(nicName == NULL)
	{
		printf("No corresponding Network Interface , Quit\n");
		return (void *)-1;
	}
	else
	{
		setsockopt(serverSocket, SOL_SOCKET, SO_BINDTODEVICE, nicName, sizeof(nicName));
	}
	if(serverSocket == -1)
	{
		cerr << "Cannot Create Server Socket, Quit." << endl;
		return (void *)-1;
	}

	if(bind(serverSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) < 0)
	{
//		cerr << "Cannot Bind Client Local Address, Quit." << endl;
		close(serverSocket);
		return (void *)-1;
	}

	int sendsize = 0;	
	int sentsize = 0;
	while(1)
	{
//		cout << "wait input " << endl;
		// wait to recv, save info of sender client
		clientAddressSize = sizeof(clientInfo);
		recvLength = recvfrom(serverSocket, (char *)&Buffer, sizeof(Buffer), 0, \
				(struct sockaddr *)&clientInfo, &clientAddressSize);
		if(recvLength > 0)
		{
			struct dhcp_packet sendBuf = build_packet(Buffer, *info, &sendsize);
			if(!strcmp(inet_ntoa(clientInfo.sin_addr),"0.0.0.0"))
				sentsize = sendto(serverSocket, (char *)&sendBuf, sendsize, 0, \
						(struct sockaddr *)&sendInfo, sizeof(sendInfo));
			else
				sentsize = sendto(serverSocket, (char *)&sendBuf, sendsize, 0, \
						(struct sockaddr *)&clientInfo, sizeof(clientInfo));
//			printf("SENT TO %s\n", inet_ntoa(clientInfo.sin_addr));
			if(sendsize < 0)
			{
				cerr << "Couldn't Reply, Quit.\n" <<endl;
				return (void *)-1;
			}
		}

	}
	close(serverSocket);
}

