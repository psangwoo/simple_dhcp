#include "dhcpark.h"

int allocInfo::addrtoalloc(int pos, int size, int clientpos)
{
	if(pos == clientpos && this->table[clientpos] != 0) // when renewal
		return pos;

	if(pos > 0 && pos < size) // when requested specific ip
	{
		if(this->table[pos] == 0) // position is allocatible
			return pos;
	}
	for(int i = 0; i < size; i++)
	{
		if(this->table[i] == 0) // i th ip is allocaitble
			return i;
	}
	return -1; // nowhere to allocate
}

/*
void savealloc(int pos, int alloctime)
{
	:q

}
*/
void allocInfo::noticealloc(int pos, int alloctime)
{
	pthread_mutex_lock(&this->mtx);
	int toalloctime = time(NULL) + alloctime - 1;
	this->table[pos] = toalloctime; 
	write_lease(this->conf.subnet, pos, toalloctime);
	pthread_mutex_unlock(&this->mtx);
	//savealloc(pos, alloctime);
}

void allocInfo::freealloc(int pos)
{
	pthread_mutex_lock(&this->mtx);
	this->table[pos] = 0;
	pthread_mutex_unlock(&this->mtx);
}

void allocInfo::mutex_init(void)
{
	pthread_mutex_init(&this->mtx, NULL);
}

string getserveraddr(string subnet)
{
	struct ifaddrs *addrs, *addr;
	getifaddrs(&addrs);
	addr = addrs;
	while(addr)
	{
		struct sockaddr_in temp;
		memcpy(&temp, addr->ifa_addr, sizeof(temp));
		string tmp = inet_ntoa(temp.sin_addr);
		if(!tmp.compare(0, tmp.size() - 1, subnet))
		{
			freeifaddrs(addrs);
			return tmp;
		}
		addr = addr->ifa_next;
	}
	freeifaddrs(addrs);
	return "";
}

char *findnic(string subnet)
{
	struct ifaddrs *addrs, *addr;
	getifaddrs(&addrs);	
	addr = addrs;
	while(addr)
	{						
		struct sockaddr_in temp;		
		memcpy(&temp, addr->ifa_addr, sizeof(temp));								
		string tmp = inet_ntoa(temp.sin_addr);
		if(!tmp.compare(0, tmp.size() - 1, subnet))
		{
			return addr->ifa_name;					
		}
		addr = addr->ifa_next;						
	}				
	freeifaddrs(addrs);

	return NULL;
}

void *timer(void *tmp)
{
	allocInfo *info = (allocInfo *)tmp;
	int size = info->conf.maxrange;
	clock_t curtime;
	while(1)
	{
//		printf("TABLE %u : \n", info->conf.serveraddr[0]);
		curtime = time(NULL);
		for(int i = 0 ; i < size; i++)
		{
			if(info->table[i] == TIME_SPECIAL)
				continue;
			else if(info->table[i] <= curtime) // dealloc ip
				info->freealloc(i);

			if(info->table[i] > 0);
//			printf("%d : %d\n", i, info->table[i] - curtime);
		}
		sleep(1);
	}
}

int *init_table(struct configData conf)
{

	int *table = new int[conf.maxrange + 1];

	for(int i = 0 ; i < conf.minrange; i++)
		table[i] = TIME_SPECIAL;
	for(int i = conf.minrange; i < conf.maxrange + 1; i++)
		table[i] = 0;

		
	return table;
}

