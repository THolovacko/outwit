#! /bin/bash

GOOS=linux GOARCH=amd64 go build -o bootstrap main.go
zip function.zip bootstrap
echo 'remember to set the handler to "bootstrap" in "Runtime settings" if not already set'
echo 'remember to set the environment variables OUTWIT_ACCESS_KEY_ID and OUTWIT_SECRET_ACCESS_KEY in "Configuration" if not already set'
echo 'remember API gateway method needs to enable Lamda Proxy Integration'
