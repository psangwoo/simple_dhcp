#!/bin/sh

if pgrep -x "dhcpark" > /dev/null; then
	echo 7
else
	echo 0
fi
