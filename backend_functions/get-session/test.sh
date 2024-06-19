#! /bin/bash

curl -X POST "https://gu0bzqber4.execute-api.us-east-1.amazonaws.com/production/get-session" \
     -H "Content-Type: application/json" \
     -d '{
           "public":  "12345",                                                                                                                                                            
           "attributes": ["party", "testtest", "blah"]
         }'
