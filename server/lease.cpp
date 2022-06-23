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
	string writefn = "/var/www/db/mysql/dhcpark/dhcpark.leases";
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
			vector<string> split_line = split(line, '\t');
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
void read_lease(allocInfo info)
//void read_lease(int *table, struct configData conf)
{
	
	ifstream readfile("/var/www/db/mysql/dhcpark/dhcpark.leases");
	if(readfile.is_open())
	{
		string line;
		while(getline(readfile, line))
		{
			if(line.compare(0, info.conf.subnet.length(), info.conf.subnet) == 0)
			{
				vector<string> tmp = split(line, '\t');
				int idx = stoi(split(tmp[0], '.')[3]);
				if(idx > info.conf.maxrange || idx < info.conf.minrange) // is this necessary ?
					continue;
				else
				{
					info.table[idx] = stol(tmp[1]);
					info.mactable[idx] = tmp[2];
				}
			}
		}
	}
	readfile.close();
}

void write_lease(string subnet, int pos, int curtime, string mac)
{
	stringstream call;
	call << "/var/www/shell/writelease.sh " << subnet << pos << " " << curtime << " " << mac << endl;
	system(call.str().c_str());
	return ;
}
