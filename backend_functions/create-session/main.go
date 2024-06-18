package main

import (
    "context"
    "github.com/aws/aws-lambda-go/lambda"
)

type MyEvent struct {
    Name string `json:"name"`
}

type MyResponse struct {
    Message string `json:"message"`
}

func HandleRequest(ctx context.Context, event MyEvent) (MyResponse, error) {
    return MyResponse{Message: "Hello, " + event.Name}, nil
}

func main() {
    lambda.Start(HandleRequest)
}
