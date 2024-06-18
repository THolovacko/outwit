#! /bin/bash

curl -X POST "https://gu0bzqber4.execute-api.us-east-1.amazonaws.com/production/create-session" \
     -H "Content-Type: application/json" \
     -d '{
           "public":  "12345thenew2",                                                                                                                                                            
           "private": "67890asd"                                                                                                                                                            
         }'
