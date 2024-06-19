#! /bin/bash

curl -X POST "https://gu0bzqber4.execute-api.us-east-1.amazonaws.com/production/update-session" \
     -H "Content-Type: application/json" \
     -d '{
           "public":  "12345",                                                                                                                                                            
           "private": "67890",
           "updates": {
             "party": "default 2"
           }
         }'
