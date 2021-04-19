#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
	./tester "http://localhost:8080"
elif [[ "$OSTYPE" == "Ubuntu"* ]]; then
	./ubuntu_tester "http://localhost:8080"
else
	echo "current OS is $OSTYPE, this only runs on OS X or Ubuntu"
fi
