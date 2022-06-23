#!/bin/bash

query="INSERT INTO leases (ip, validt, mac) VALUES ('$1', $2, '$3')
		ON DUPLICATE KEY UPDATE ip = '$1', validt= '$2', mac = '$3' ;"

mysql -uroot -p{1234} -D dhcpark -e "${query}"
