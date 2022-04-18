#include "dhcpark.h"


int main(void)
{
	allocInfo contrl;

	contrl.conf = readfromconfig();
	contrl.table = init_table(contrl.conf);
	contrl.mutex_init();
	pthread_t pthr[2];

	pthread_create(&pthr[0], NULL, timer, (void *)&contrl);
	pthread_create(&pthr[1], NULL, dhcp, (void *)&contrl);

	int status;
	
	pthread_join(pthr[0], (void **)&status);
	pthread_join(pthr[1], (void **)&status);

	return 0;
}
