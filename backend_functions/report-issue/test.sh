#! /bin/bash

curl -X POST "https://gu0bzqber4.execute-api.us-east-1.amazonaws.com/production/report-issue" \
     -H "Content-Type: application/json" \
     -d '{
           "contact": "tom.holovacko@gmail.com",                                                                                                                                                            
           "issue": "This is a test issue"                                                                                                                                                            
         }'
