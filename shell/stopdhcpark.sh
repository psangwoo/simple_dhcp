#!/bin/sh

ps -ef | grep /var/www/server/dhcpark | awk '{print $2}' | xargs kill -9
