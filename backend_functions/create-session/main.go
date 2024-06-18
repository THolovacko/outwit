package main

import (
  "context"
  "encoding/json"
  "fmt"
  "os"

  "github.com/aws/aws-lambda-go/events"
  "github.com/aws/aws-lambda-go/lambda"
  "github.com/aws/aws-sdk-go/aws"
  "github.com/aws/aws-sdk-go/aws/credentials"
  "github.com/aws/aws-sdk-go/aws/session"
  "github.com/aws/aws-sdk-go/service/dynamodb"
)

type Request struct {
  Public  string `json:"public"`
  Private string `json:"private"`
}

type Response struct {
  Available bool   `json:"available"`
  Private   string `json:"private,omitempty"`
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

  id := req.Public

  input := &dynamodb.GetItemInput{
    TableName: aws.String("Sessions"),
    Key: map[string]*dynamodb.AttributeValue{
      "id": {
        S: aws.String(id),
      },
    },
  }

  result, err := dynamoDBClient.GetItem(input)
  if err != nil {
    return events.APIGatewayProxyResponse{StatusCode: 500}, err
  }

  if result.Item != nil {
    return events.APIGatewayProxyResponse{
      StatusCode: 200,
      Body:       `{"available": false}`,
    }, nil
  }

  itemInput := &dynamodb.PutItemInput{
    TableName: aws.String("Sessions"),
    Item: map[string]*dynamodb.AttributeValue{
      "id": {
        S: aws.String(id),
      },
      "private": {
        S: aws.String(req.Private),
      },
    },
  }

  _, err = dynamoDBClient.PutItem(itemInput)
  if err != nil {
    return events.APIGatewayProxyResponse{StatusCode: 500}, err
  }

  resp := Response{Available: true}
  respBody, err := json.Marshal(resp)
  if err != nil {
    return events.APIGatewayProxyResponse{StatusCode: 500}, err
  }

  return events.APIGatewayProxyResponse{
    StatusCode: 200,
    Body:       string(respBody),
  }, nil
}

func main() {
  lambda.Start(handler)
}
