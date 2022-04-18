#include "dhcpark.h"



map<string, int> map_create(void)
{
	map <string, int> ret;
	ret["server"] = SERVER;
	ret["domain-name-servers"] = DNS;
	ret["domain-name"] = DNSNAME;
	ret["lease-time"] = LEASETIME;
	ret["subnet"] = SUBNET;
	ret["netmask"] = NETMASK;
	ret["range-min"] = MIN_RANGE;
	ret["range-max"] = MAX_RANGE;
	ret["broadcast-address"] = BROADCAST;
	ret["gateway"] = GATEWAY;
	return ret;

}
vector<string> split(string str, char Delimeter)
{
	stringstream iss(str);
	string buffer;

	vector<string> ret;

	while(getline(iss, buffer, Delimeter))
	{
		ret.push_back(buffer);
	}
	return ret;
}
struct configData readfromconfig(void)
{

	ifstream configFile("dhcpark.conf");
	map<string, int> configmap = map_create();
	struct configData settings;

	if(configFile.is_open())
	{
		string line;
		while(getline(configFile, line))
		{
			if(line[0] == '#' ) // if line = comment
				continue;
			if(line[0] == '\0') // no data, newline
				continue;
			line.erase(remove(line.begin(), line.end(), ' '), line.end()); // remove blank
			vector<string> split_line = split(line, '=');

			switch(configmap[split_line[0]])
			{
				case SERVER:
					{
						vector<string> tmp = split(split_line[1], '.');
						for(int i = 0; i < tmp.size(); i++)
							settings.serveraddr[i] = stoi(tmp[i]);
						break;
					}
				case DNS:
					{
						vector<string> tmp = split(split_line[1], '.');
						for(int i = 0; i < tmp.size(); i++)
							settings.dns[i] = stoi(tmp[i]);
						break;
					}
				case DNSNAME:
					{
						settings.domain = split_line[1];
						break;
					}
				case LEASETIME:
					{
						int tmp = stoi(split_line[1]);
						for(int i = 3 ; i >= 0 ; i--)
						{
							settings.leasetime[i] = tmp % 256;
							tmp /= 256;
							printf("Time : %d\n", settings.leasetime[i]);
						}
						break;
					}
				case SUBNET:
					{
						settings.subnet = split_line[1];
						while(settings.subnet.back() != '.')
							settings.subnet.pop_back();
						break;
					}
				case NETMASK:
					{
						vector<string> tmp = split(split_line[1], '.');
						for(int i = 0; i < tmp.size(); i++)
							settings.netmask[i] = stoi(tmp[i]);
						break;
					}
				case MIN_RANGE:
					{
						settings.minrange = stoi(split(split_line[1], '.')[3]);
						break;
					}
				case MAX_RANGE:
					{
						settings.maxrange = stoi(split(split_line[1], '.')[3]);
						break;
					}
				case BROADCAST:
					{
						vector<string> tmp = split(split_line[1], '.');
						for(int i = 0; i < tmp.size(); i++)
							settings.broadcast[i] = stoi(tmp[i]);
						break;
					}
				case GATEWAY:
					{
						vector<string> tmp = split(split_line[1], '.');
						for(int i = 0; i < tmp.size(); i++)
							settings.gateway[i] = stoi(tmp[i]);
						break;
					}
			}
			
		}
		configFile.close();
	}
	for(int i = 0 ; i < 4; i++)
		printf("%u.", settings.serveraddr[i]);
	cout << endl;
	for(int i = 0 ; i < 4; i++)
		printf("%u ", settings.leasetime[i]);
	cout << endl;
	cout << settings.minrange << " : " << settings.maxrange << endl;
	cout << settings.domain << endl;
	cout << settings.subnet << endl;
	for(int i = 0 ; i <  4; i++)
		printf("%u.", settings.dns[i]);
	cout << endl;
	for(int i = 0 ; i <  4; i++)
		printf("%u.", settings.gateway[i]);
	cout << endl;
	for(int i = 0 ; i <  4; i++)
		printf("%u.", settings.broadcast[i]);
	cout << endl;
	for(int i = 0 ; i <  4; i++)
		printf("%u.", settings.netmask[i]);
	cout << endl;

	return settings;
}
