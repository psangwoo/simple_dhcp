#!/bin/bash

if [ -f "/var/www/db/mysql/dhcpark/dhcpark.leases" ]; then
	rm -f /var/www/db/mysql/dhcpark/dhcpark.leases
fi
utime=`date +%s`

query1="DELETE FROM leases
	WHERE validt < ${utime}
;"

mysql -uroot -p1234 -D dhcpark -e "${query1}"

query2="SELECT * 
	FROM leases
	INTO OUTFILE 'dhcpark.leases'
	LINES TERMINATED BY '\n'
;"

mysql -uroot -p1234 -D dhcpark -e "${query2}"
