import os, sys

if __name__== '__main__':
	method = sys.argv[1]
	username = sys.argv[2]

if method == "run" :
	os.system("sh /var/www/shell/makeconfig.sh %s".format(username))
	os.system("sh /var/www/shell/makelease.sh %s".format(username))
	os.system("/var/www/server/dhcpark > /dev/null &");
	sys.exit(1)
elif method == "stop" :
	os.system("sh /var/www/shell/stopdhcpark.sh");
	sys.exit(1)
else:
	sys.exit(0)

