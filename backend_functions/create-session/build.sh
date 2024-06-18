#! /bin/bash

GOOS=linux GOARCH=amd64 go build -o bootstrap main.go
zip function.zip bootstrap
echo 'remember to set the handler to "bootstrap" in "Runtime settings" if not already set'
