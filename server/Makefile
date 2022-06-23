main : config.o table.o packet.o lease.o main.o
		g++ -lpthread config.o table.o packet.o lease.o main.o -o ./dhcpark
config.o : dhcpark.h config.cpp
				g++ -c --std=c++11 config.cpp
table.o : dhcpark.h table.cpp
			g++ -c --std=c++11 table.cpp
packet.o : dhcpark.h packet.cpp
			g++ -c --std=c++11 packet.cpp
lease.o : dhcpark.h lease.cpp
			g++ -c --std=c++11 lease.cpp
main.o : dhcpark.h main.cpp
			g++ -c --std=c++11 -lpthread main.cpp
clean : 
		rm -f dhcpark config.o table.o packet.o lease.o main.o
