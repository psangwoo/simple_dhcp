#include "dhcpark.h"

int allocInfo::addrtoalloc(int pos, int size)
{

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
	this->table[pos] = time(NULL) + alloctime;
	cout << "alloced at " << pos << " of time " << this->table[pos] << " Amount : " << alloctime << endl;
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


void readfromlease(int table[])
{
	/*
	ifstream leaseFile("dhcpark.leases");
	string line;
	while(getline(leaseFile, line))
	{
		if(!strncmp(line.c_str(), "Block", 5))
			;
	}
	close(leaseFile);
	*/
}

void *timer(void *tmp)
{
	allocInfo *info = (allocInfo *)tmp;
	int size = info->conf.maxrange;
	clock_t curtime;
	while(1)
	{
		cout << "TABLE : " << endl;
		curtime = time(NULL);
		for(int i = 0 ; i < size; i++)
		{
			if(info->table[i] == TIME_SPECIAL)
				continue;
			else if(info->table[i] <= curtime) // dealloc ip
				info->freealloc(i);

			if(info->table[i] > 0)
				printf("%d : %d\n", i, info->table[i] - curtime);
		}
		sleep(1);
	}
}


int *init_table(struct configData conf)
{
	int n = 153;
	int m = 3;

	int *table = new int[conf.maxrange + 1];

	for(int i = 0 ; i < conf.minrange; i++)
		table[i] = TIME_SPECIAL;

	readfromlease(table);
		
	return table;
}

