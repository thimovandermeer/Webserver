#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
	./webserv "configs/tester_OS_X.conf"
elif [[ "$OSTYPE" == "Ubuntu"* ]]; then
	./webserv "configs/tester_Ubuntu.conf"
else
	echo "current OS is $OSTYPE, this only runs on OS X or Ubuntu"
fi
