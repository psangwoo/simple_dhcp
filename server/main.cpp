#include "dhcpark.h"


int main(void)
{
	vector<allocInfo> contrl;
	vector<struct configData> tmp = readfromconfig();
	init_lease();
	for(int i = 0; i < tmp.size(); i++)
	{
		allocInfo tmpcls = {};
		tmpcls.conf = tmp[i];
		char *interface = findnic(tmpcls.conf.subnet);
		if(interface == NULL)
		{
			cout << "No corresponding subnet found." << tmpcls.conf.subnet << endl;
			continue;
		}

		init_table(&tmpcls);
		tmpcls.mutex_init();
		read_lease(tmpcls);
		contrl.push_back(tmpcls);
	}
	pthread_t *pthr = new pthread_t[tmp.size()];
	for(int i = 0 ; i < contrl.size(); i++)		
	{
		pthread_create(&pthr[i], NULL, timer, (void *)&contrl[i]);
		pthread_create(&pthr[i], NULL, dhcp, (void *)&contrl[i]);
	}
	int status;
	for(int i = 0; i < contrl.size(); i++)	
		pthread_join(pthr[i], (void **)&status);
	return 0;
}
