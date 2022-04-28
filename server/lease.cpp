#include "dhcpark.h"

bool isnum(string line)
{
	if(!line.length())
		return false;
	for( auto c : line)
	{
		if(!isdigit(c))
			return false;
	}
	return true;
}

void init_lease()
{
	string writefn = "dhcpark.leases";
	string readfn = writefn + ".old";
	rename(writefn.c_str(), readfn.c_str());

	ifstream readlease(readfn);
	ofstream writelease(writefn);
	
	if(readlease.is_open() && writelease.is_open())
	{
		writelease << "# ( ALLOCATED IP ADDRESS ) ( EXPIRATION TIME ) " << endl;
		string line;
		long curtime = time(NULL);
		while(getline(readlease, line))
		{
			if(line[0] == '#')
				continue;
			vector<string> split_line = split(line, ' ');
			if(isnum(split_line[1]))
			{
				if(stol(split_line[1]) > curtime)
					writelease << line << endl;
			}
		}
	}
	remove(readfn.c_str());
	readlease.close();
	writelease.close();
}
void read_lease(int *table, struct configData conf)
{
	ifstream readfile("dhcpark.leases");

	if(readfile.is_open())
	{
		string line;
		while(getline(readfile, line))
		{
			if(line.compare(0, conf.subnet.length(), conf.subnet) == 0)
			{
				vector<string> tmp = split(line, ' ');
				int idx = stoi(split(tmp[0], '.')[3]);
				if(idx > conf.maxrange || idx < conf.minrange) // is this necessary ?
					continue;
				else
				{
					table[idx] = stol(tmp[1]);
				}
			}
		}
	}
	readfile.close();
}

void write_lease(string subnet, int pos, int curtime)
{
	ofstream leasefile("dhcpark.leases", ios_base::app);

	leasefile << subnet << pos << " " << curtime << endl;
	
	leasefile.close();
}
