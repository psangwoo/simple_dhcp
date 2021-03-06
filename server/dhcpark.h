#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <ifaddrs.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>

#define PORT_DHCP_SERVER 67
#define PORT_DHCP_CLIENT 68
#define DHCP_SERVERNAME_LENGTH 64
#define DHCP_BOOTFILE_LENGTH 128
#define DHCP_OPTION_LENGTH 312
#define TIME_TRUE 1
#define TIME_FALSE 0
#define TIME_SPECIAL -6586
using namespace std;

using std::cout;
using std::cerr;
using std::endl;

enum configVariables
{
    DNS = 0,
	DNSNAME,	
	LEASETIME,		
	SUBNET,		
	NETMASK,		
	MIN_RANGE,		
	MAX_RANGE,	
	BROADCAST,
	GATEWAY
};

enum messageTypes
{
	DISCOVER = 1,
	OFFER,
	REQUEST,
	DECLINE,
	ACK,
	NACK,
	RELEASE,
	INFORM
};


struct configData
{
	unsigned char serveraddr[4];
	unsigned char leasetime[4];
	int minrange;
	int maxrange;
	string domain;
	string subnet;
	unsigned char dns[4];
	unsigned char gateway[4];
	unsigned char broadcast[4];	
	unsigned char netmask[4];
};

class allocInfo
{
	public:
		int *table;
		string *mactable;
		struct configData conf;
		int addrtoalloc(int , int , int , string);
		void noticealloc(int , int , string );
		void freealloc(int );
		void mutex_init();
	private:
		pthread_mutex_t mtx;
};

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

map<string, int> map_create(void);
vector<string> split(string , char );
vector<struct configData> readfromconfig(void);

void readfromlease(int table[]);
string getserveraddr(string );
char *findnic(string );
void *timer(void * );
string convert_tomac(unsigned char _[16]);
void init_table(allocInfo *);

struct dhcp_packet build_packet(struct dhcp_packet , allocInfo );
void *dhcp(void * );

void init_lease();
void read_lease(allocInfo );
void write_lease(string , int , int , string );

