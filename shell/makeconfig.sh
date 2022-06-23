#!/bin/bash

if [ -f "/var/www/db/mysql/dhcpark/dhcpark.conf" ]; then
	rm -f /var/www/db/mysql/dhcpark/dhcpark.conf
fi
query="SELECT CONCAT('subnet = ', subnet), CONCAT('minrange = ', minrange), CONCAT('maxrange = ', maxrange), CONCAT('domain = ', domain), CONCAT('dns = ', dns), CONCAT('leasetime = ', leasetime), CONCAT('mask = ', mask), CONCAT('gateway = ', gateway), CONCAT('broadcast = ', broadcast)
FROM config WHERE username = '$1' 
INTO OUTFILE 'dhcpark.conf'
FIELDS TERMINATED BY '\n'
LINES STARTING BY 'interface \n' TERMINATED BY '\nend\n\n'
;"

mysql -uroot -p1234 -D dhcpark -e "${query}"
