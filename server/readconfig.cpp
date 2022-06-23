#include "dhcpark.h"


map<string, int> map_create(void)
{
	map <string, int> ret;
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

vector<struct configData> readfromconfig(void)
{

	ifstream configFile("dhcpark.conf");
	map<string, int> configmap = map_create();
	vector<struct configData> ret;
	

	if(configFile.is_open())
	{
		string line;
		while(getline(configFile, line))
		{
			if(line[0] == '#' ) // if line = comment
				continue;
			if(line[0] == '\0') // no data, newline
				continue;
			if(!strncmp(line.c_str(), "interface", 9))
			{

				struct configData setting;
				while(getline(configFile, line))
				{

					if(!strncmp(line.c_str(), "end", 3))
							break;

					line.erase(remove(line.begin(), line.end(), ' '), line.end()); // remove blank
					if(line[0] == '#' ) // if line = comment
						continue;
					if(line[0] == '\0') // no data, newline
						continue;

					vector<string> split_line = split(line, '=');
		
					switch(configmap[split_line[0]])
					{
						case DNS:
							{
								vector<string> tmp = split(split_line[1], '.');
								for(int i = 0; i < tmp.size(); i++)
									setting.dns[i] = stoi(tmp[i]);
								break;
							}
						case DNSNAME:
							{
								setting.domain = split_line[1];
								break;
							}
						case LEASETIME:
							{
								int tmp = stoi(split_line[1]);
								for(int i = 3 ; i >= 0 ; i--)
								{
									setting.leasetime[i] = tmp % 256;
									tmp /= 256;
								}
								break;
							}
						case SUBNET:
							{
								setting.subnet = split_line[1];
								while(setting.subnet.back() != '.')
									setting.subnet.pop_back();
								break;
							}
						case NETMASK:
							{
								vector<string> tmp = split(split_line[1], '.');
								for(int i = 0; i < tmp.size(); i++)
									setting.netmask[i] = stoi(tmp[i]);
								break;
							}
						case MIN_RANGE:
							{
								setting.minrange = stoi(split(split_line[1], '.')[3]);
								break;
							}
						case MAX_RANGE:
							{
								setting.maxrange = stoi(split(split_line[1], '.')[3]);
								break;
							}
						case BROADCAST:
							{
								vector<string> tmp = split(split_line[1], '.');
								for(int i = 0; i < tmp.size(); i++)
									setting.broadcast[i] = stoi(tmp[i]);
								break;
							}
						case GATEWAY:
							{
								vector<string> tmp = split(split_line[1], '.');
								for(int i = 0; i < tmp.size(); i++)
									setting.gateway[i] = stoi(tmp[i]);
								break;
							}
					}	
				}
				vector<string> myaddr = split(getserveraddr(setting.subnet), '.');
				for(int i = 0; i < myaddr.size(); i++)
				{
					setting.serveraddr[i] = stoi(myaddr[i]);
				}
				
				ret.push_back(setting);
			}

		}

		configFile.close();

	}

	return ret;
}
