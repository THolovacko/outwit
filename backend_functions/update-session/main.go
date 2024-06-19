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
  Private string `json:"private"`
  Updates map[string]interface{} `json:"updates"`
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

  id := req.Public

  getInput := &dynamodb.GetItemInput{
    TableName: aws.String("Sessions"),
    Key: map[string]*dynamodb.AttributeValue{
      "id": {
        S: aws.String(id),
      },
    },
    ProjectionExpression: aws.String("private_id"),
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

  currentPrivate := result.Item["private_id"].S
  if currentPrivate == nil || *currentPrivate != req.Private {
    log.Printf("Private value does not match")
    return events.APIGatewayProxyResponse{StatusCode: 400, Body: "Private value does not match"}, nil
  }

  updateExpression := "set"
  expressionAttributeValues := make(map[string]*dynamodb.AttributeValue)
  for key, value := range req.Updates {
    updateExpression += fmt.Sprintf(" %s = :%s,", key, key)
    expressionAttributeValues[fmt.Sprintf(":%s", key)] = &dynamodb.AttributeValue{S: aws.String(fmt.Sprintf("%v", value))}
  }
  updateExpression = updateExpression[:len(updateExpression)-1] // remove trailing comma

  updateItemInput := &dynamodb.UpdateItemInput{
    TableName: aws.String("Sessions"),
    Key: map[string]*dynamodb.AttributeValue{
      "id": {S: aws.String(req.Public)},
    },
    UpdateExpression:          aws.String(updateExpression),
    ExpressionAttributeValues: expressionAttributeValues,
  }

  _, err = dynamoDBClient.UpdateItem(updateItemInput)
  if err != nil {
    log.Printf("Failed to update session: %v", err)
    return events.APIGatewayProxyResponse{StatusCode: 500, Body: "Failed to update item"}, nil
  }

  return events.APIGatewayProxyResponse{StatusCode: 200, Body: "Session updated successfully"}, nil
}

func main() {
  lambda.Start(handler)
}
