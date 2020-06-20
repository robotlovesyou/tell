package main

import (
	"calculon/calculator"
	"context"
	"log"
	"os"
	"sync"
	"time"
)

type calculon struct {
	current int64
	mutex sync.Mutex
}

func (c *calculon) Add(ctx context.Context, arg calculator.Addend) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.current += arg.Value
	return calculator.Result{Value: c.current}, nil
}

func (c *calculon) Subtract(ctx context.Context, arg calculator.Subtrahend) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.current -= arg.Value
	return calculator.Result{c.current}, nil
}

// Multiplies the current value by the argument
func (c *calculon) Multiply(ctx context.Context, arg calculator.Multiplicand) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.current *= arg.Value
	return calculator.Result{c.current}, nil
}

// Divides the current value by the argument
func (c *calculon) Divide(ctx context.Context, arg calculator.Divisor) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	if arg.Value == 0 {
		return calculator.Result{}, calculator.InvalidArgumentError(map[string]string{"value": "cannot be 0"})
	}
	c.current /= arg.Value
	return calculator.Result{c.current}, nil
}

// Reads the current value
func (c *calculon) Read(ctx context.Context, arg calculator.Empty) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	return calculator.Result{c.current}, nil
}
// Resets the current value to 0
func (c *calculon) Reset(ctx context.Context, arg calculator.Empty) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.current = 0
	return calculator.Result{c.current }, nil
}

func main() {
	service := new(calculon)
	server := calculator.NewTellServer()
	calculator.RegisterCalculatorServer(service, server)
	go func() {
		server.Start(8000)
	}()
	time.Sleep(time.Second)

	client := calculator.NewCalculatorClient("localhost:8000")
	res, err := client.Add(context.Background(), calculator.Addend{Value: 10})
	if err != nil {
		log.Fatal("Could not add 10: %v", err)
	}
	log.Printf("after adding 10 the result is: %d", res.Value)

	res, err = client.Multiply(context.Background(), calculator.Multiplicand{Value: 100})
	if err != nil {
		log.Fatal("Could not multiply by 100: %v", err)
	}
	log.Printf("after multiplcation by 100 the result is: %d", res.Value)


	res, err = client.Divide(context.Background(), calculator.Divisor{Value: 2})
	if err != nil {
		log.Fatal("Could not divide by 2: %v", err)
	}
	log.Printf("after division by 2 the result is: %d", res.Value)

	res, err = client.Subtract(context.Background(), calculator.Subtrahend{Value: 1})
	if err != nil {
		log.Fatal("Could not subtract 1: %v", err)
	}
	log.Printf("after subtracting 1 the result is: %d", res.Value)

	res, err = client.Read(context.Background(), calculator.Empty{})
	if err != nil {
		log.Fatal("Could not read: %v", err)
	}
	log.Printf("reading calculator value: %d", res.Value)

	res, err = client.Reset(context.Background(), calculator.Empty{})
	if err != nil {
		log.Fatal("Could not reset: %v", err)
	}
	log.Printf("calculator is reset to: %d", res.Value)
	os.Exit(0)
}
