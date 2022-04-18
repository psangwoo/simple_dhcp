main : readconfig.o table.o packet.o main.o
		g++ -lpthread readconfig.o table.o packet.o main.o -o ./dhcpark
readconfig.o : dhcpark.h readconfig.cpp
				g++ -c --std=c++11 readconfig.cpp
table.o : dhcpark.h table.cpp
			g++ -c --std=c++11 table.cpp
packet.o : dhcpark.h packet.cpp
			g++ -c --std=c++11 packet.cpp
main.o : dhcpark.h main.cpp
			g++ -c --std=c++11 -lpthread main.cpp
clean : 
		rm -f dhcpark readconfig.o table.o packet.o main.o
