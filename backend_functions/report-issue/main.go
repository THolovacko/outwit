package main

import (
  "context"
  "encoding/json"
  "fmt"
  "os"
  "log"
  "time"

  "github.com/aws/aws-lambda-go/events"
  "github.com/aws/aws-lambda-go/lambda"
  "github.com/aws/aws-sdk-go/aws"
  "github.com/aws/aws-sdk-go/aws/credentials"
  "github.com/aws/aws-sdk-go/aws/session"
  "github.com/aws/aws-sdk-go/service/dynamodb"
)

type Request struct {
  Contact string `json:"contact"`
  Issue   string `json:"issue"`
}

var dynamoDBClient *dynamodb.DynamoDB

func init() {
  sess, err := session.NewSession(&aws.Config{
    Region: aws.String(os.Getenv("AWS_REGION")),
    Credentials: credentials.NewStaticCredentials(os.Getenv("OUTWIT_ACCESS_KEY_ID"), os.Getenv("OUTWIT_SECRET_ACCESS_KEY"), ""),
  })
  if err != nil {
    fmt.Println("Error creating session:", err)
    os.Exit(1)
  }

  dynamoDBClient = dynamodb.New(sess)
}

func handler(ctx context.Context, request events.APIGatewayProxyRequest) (events.APIGatewayProxyResponse, error) {
  var req Request
  err := json.Unmarshal([]byte(request.Body), &req)
  if err != nil {
    return events.APIGatewayProxyResponse{StatusCode: 400}, err
  }

  now := time.Now().UTC()
  key := fmt.Sprintf("%s_%s", req.Contact, now.Format("2006-01-02T15:04:05"))

  input := &dynamodb.PutItemInput{
    TableName: aws.String("Reported_Issues"),
    Item: map[string]*dynamodb.AttributeValue{
      "contact_datetime": {
        S: aws.String(key),
      },
      "issue": {
        S: aws.String(req.Issue),
      },
    },
  }

  _, err = dynamoDBClient.PutItem(input)
  if err != nil {
    log.Printf("Failed to put item into DynamoDB: %v", err)
    return events.APIGatewayProxyResponse{StatusCode: 500, Body: "Failed to create issue"}, err
  }

  responseBody := fmt.Sprintf("Successfully added issue for contact %s", req.Contact)
  return events.APIGatewayProxyResponse{StatusCode: 200, Body: responseBody}, nil
}

func main() {
  lambda.Start(handler)
}
