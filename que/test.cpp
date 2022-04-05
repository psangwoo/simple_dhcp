#include "dhcpark.h"

void* sender(void *w)
{
	struct sockaddr_in receiverInfo = {};

	socklen_t serverAddressSize = 0;
	
	receiverInfo.sin_family = AF_INET;
	receiverInfo.sin_port = htons(27001);

	int senderSocket = socket(AF_INET, SOCK_DGRAM, 0);
	int option = 1;
	setsockopt(senderSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if(senderSocket == -1)
	{
		cerr << "Cannot Create Client Socket, Quit. " << endl;

		return (void *)-1;
		
	}
	//while(!que.empty())
	while(1)
	{
//		QueueElem *received = new QueueElem;
//		*received = que.front();
//		que.pop();
		struct packet sendp = {};
		sendp.ack = 1;
//		sendp.data = "Received";
		cin >> sendp.data;	
		receiverInfo.sin_addr.s_addr = inet_addr("192.168.0.2");

		int sendsize = sendto(senderSocket, (void *)&sendp, sizeof(sendp), 0, (struct sockaddr *)&receiverInfo, sizeof(receiverInfo));
		if(sendsize < 0)
		{
			cerr << " Could Not send .. Quit.\n" << endl;
			return (void *)-1;
		}
	}
}
void* receiver(void *w)
{
	struct packet Buffer;
	struct sockaddr_in senderInfo = {};
	socklen_t senderAddrSize = 0;

	senderInfo.sin_family = AF_INET;
	senderInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	senderInfo.sin_port = htons(27001);

	int receiverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int option = 1;
	setsockopt(receiverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if(receiverSocket == -1)
	{
		cerr << "Cannot Create Server Socket, Quit." << endl;
		return (void *)-1;
	}
	if(bind(receiverSocket, (struct sockaddr *)&senderInfo, sizeof(senderInfo)) < 0)
	{
		cerr << "Cannot Bind Local Address, Quit." << endl;
		return (void *)-1;
	}
	int recvLength = 0;
	senderAddrSize = sizeof(senderInfo);
	while(1)
	{
		recvLength = recvfrom(receiverSocket, (char *)&Buffer, sizeof(Buffer), 0, (struct sockaddr *)&senderInfo, &senderAddrSize);
		if(recvLength > 0)
		{
			QueueElem *newElem = new QueueElem;
			newElem->packet = Buffer;
			newElem->addr = senderInfo.sin_addr.s_addr;

			que.push(*newElem);
		}
	}
}
int main(void)
{
	pthread_t thr[2];
	int tmp= 0;
	pthread_create(&thr[0], NULL, sender, NULL );
	pthread_create(&thr[1], NULL, receiver, NULL );
	int status;
	pthread_join(thr[0], (void **)status);
	pthread_join(thr[1], (void **)status);
	return 0;
}
