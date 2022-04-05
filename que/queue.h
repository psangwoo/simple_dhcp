#include <queue>
using namespace std;

struct packet
{
	int ack;
	string data;
};

typedef struct qelem
{
	struct packet packet;
	u_long addr;
}QueueElem;

queue<QueueElem> que;
