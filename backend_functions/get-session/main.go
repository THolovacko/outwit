package main

import (
  "context"
  "encoding/json"
  "fmt"
  "os"
  "log"

  "github.com/aws/aws-lambda-go/events"
  "github.com/aws/aws-lambda-go/lambda"
  "github.com/aws/aws-sdk-go/aws"
  "github.com/aws/aws-sdk-go/aws/credentials"
  "github.com/aws/aws-sdk-go/aws/session"
  "github.com/aws/aws-sdk-go/service/dynamodb"
)

type Request struct {
  Public  string `json:"public"`
  Attributes []string `json:"attributes"`
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
    log.Printf("Failed to unmarshal JSON request: %v", err)
    return events.APIGatewayProxyResponse{StatusCode: 400}, err
  }

  if req.Public == "" || len(req.Attributes) == 0 {
      log.Printf("ID or Attributes are missing")
      return events.APIGatewayProxyResponse{StatusCode: 400, Body: "ID or Attributes are missing"}, nil
  }

  id := req.Public
  projectionExpression := ""
  expressionAttributeNames := make(map[string]*string)

  for _, attr := range req.Attributes {
    if attr != "private_id" {
      alias := fmt.Sprintf("#%s", attr)
      projectionExpression += alias + ","
      expressionAttributeNames[alias] = aws.String(attr)
    } else {
      log.Printf("Attribute 'private_id' is not allowed")
      return events.APIGatewayProxyResponse{StatusCode: 400, Body: "Attribute 'private_id' is not allowed"}, nil
    }
  }
  projectionExpression = projectionExpression[:len(projectionExpression)-1] // remove the trailing comma from the projection expression

  getInput := &dynamodb.GetItemInput{
    TableName: aws.String("Sessions"),
    Key: map[string]*dynamodb.AttributeValue{
      "id": {
        S: aws.String(id),
      },
    },
    ProjectionExpression: aws.String(projectionExpression),
    ExpressionAttributeNames: expressionAttributeNames,
  }

  result, err := dynamoDBClient.GetItem(getInput)
  if err != nil {
    log.Printf("Failed to get item: %v", err)
    return events.APIGatewayProxyResponse{StatusCode: 500}, err
  }

  if result.Item == nil {
    log.Printf("Item not found")
    return events.APIGatewayProxyResponse{StatusCode: 400, Body: "Item not found"}, nil
  }

  filteredAttributes := make(map[string]string)
  for _, attr := range req.Attributes {
    if av, ok := result.Item[attr]; ok && av.S != nil {
      filteredAttributes[attr] = *av.S
    }
  }

  responseBody, err := json.Marshal(filteredAttributes)
  if err != nil {
    log.Printf("Failed to marshal result: %v", err)
    return events.APIGatewayProxyResponse{StatusCode: 500}, err
  }

  return events.APIGatewayProxyResponse{StatusCode: 200, Body: string(responseBody)}, nil
}

func main() {
  lambda.Start(handler)
}
