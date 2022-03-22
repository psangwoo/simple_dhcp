#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>

#define PORT_DHCP_SERVER 67
#define PORT_DHCP_CLIENT 68
#define DHCP_SERVERNAME_LENGTH 64
#define DHCP_BOOTFILE_LENGTH 128
#define DHCP_OPTION_LENGTH 1224
using namespace std;

using std::cout;
using std::cerr;
using std::endl;

struct dhcp_packet
{
	u_int8_t op; 								/*   0 : Opcode */
	u_int8_t hw_type;							/*   1 : Hardware Type */
	u_int8_t hw_addr_len;						/*   2 : Hardware Address Length */
	u_int8_t hop_count; 						/*   3 : Hop Count */
	u_int32_t tx_id;							/*   4 : Transaction Identifier */
	u_int16_t nsecs;							/*   8 : Number of Seconds */
	u_int16_t flags;							/*  10 : Flag Bit */
	struct in_addr cipaddr;						/*  12 : Client IP Address */
	struct in_addr yipaddr;						/*  16 : Your IP Address */
	struct in_addr sipaddr;						/*  20 : Server IP Address */
	struct in_addr gipaddr;						/*  24 : Gateway IP Address */
	unsigned char chwaddr[16];					/*  28 : Client MAC Address */
	char sname[DHCP_SERVERNAME_LENGTH]; 		/*  44 : Server Name */
	char bfile[DHCP_BOOTFILE_LENGTH];			/* 108 : Boot File Name */
	unsigned char options[DHCP_OPTION_LENGTH];	/* 236 : Options */
};


