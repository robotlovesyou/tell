package main

import (
	"calculon/calculator"
	"context"
	"log"
	"os"
	"sync"
	"time"
)

// calculon complies with the CalculatorServer interface in the generated code.
type calculon struct {
	current int64
	memories map[string]calculator.Memory
	mutex sync.Mutex
}

func (c *calculon) Add(ctx context.Context, arg calculator.Operand) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.current += arg.Value
	return calculator.Result{Value: c.current}, nil
}

func (c *calculon) Subtract(ctx context.Context, arg calculator.Operand) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.current -= arg.Value
	return calculator.Result{c.current}, nil
}

// Multiplies the current value by the argument
func (c *calculon) Multiply(ctx context.Context, arg calculator.Operand) (calculator.Result, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.current *= arg.Value
	return calculator.Result{c.current}, nil
}

// Divides the current value by the argument
func (c *calculon) Divide(ctx context.Context, arg calculator.Operand) (calculator.Result, error) {
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

func (c *calculon) Memorize(ctx context.Context, arg calculator.Tag) (calculator.Memory, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	memory := calculator.Memory{
		Name: arg.Name,
		At: time.Now().UTC(),
		Value: c.current,
	}
	c.memories[memory.Name] = memory
	return memory, nil
}

func (c *calculon) MemoryDump(ctx context.Context, arg calculator.Empty) (calculator.Memories, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()
	var memories calculator.Memories
	for _, memory := range c.memories {
		memories.Items = append(memories.Items, memory)
	}
	return memories, nil
}

func main() {
    // Create an instance of the calculon server
	service := &calculon{
		memories: map[string]calculator.Memory{},
	}

    // Create a new TellServer and then register the calculator server with the TellServer
	server := calculator.NewTellServer()
	calculator.RegisterCalculatorServer(service, server)

	// Start the calculator server in a goroutine so it doesn't block
	go func() {
		server.Start(8000)
	}()
	time.Sleep(time.Second)

    // Create an instance of the generated Calculator client
	client := calculator.NewCalculatorClient("localhost:8000")
	res, err := client.Add(context.Background(), calculator.Operand{Value: 10})
	if err != nil {
		log.Fatal("Could not add 10: %v", err)
	}
	log.Printf("after adding 10 the result is: %d", res.Value)

    // Call a sequence of RPC methods on the calculator server and output the result each time
	res, err = client.Multiply(context.Background(), calculator.Operand{Value: 100})
	if err != nil {
		log.Fatal("Could not multiply by 100: %v", err)
	}
	log.Printf("after multiplcation by 100 the result is: %d", res.Value)


	res, err = client.Divide(context.Background(), calculator.Operand{Value: 2})
	if err != nil {
		log.Fatal("Could not divide by 2: %v", err)
	}
	log.Printf("after division by 2 the result is: %d", res.Value)

	res, err = client.Subtract(context.Background(), calculator.Operand{Value: 1})
	if err != nil {
		log.Fatal("Could not subtract 1: %v", err)
	}
	log.Printf("after subtracting 1 the result is: %d", res.Value)

	mem, err := client.Memorize(context.Background(), calculator.Tag{Name: "first"})
	if err != nil {
		log.Fatal("Could not memorize with tag first: %v", err)
	}
	log.Printf("memory first: %v", mem)

	res, err = client.Divide(context.Background(), calculator.Operand{Value: 2})
	if err != nil {
		log.Fatal("Could not divide by 2: %v", err)
	}
	log.Printf("after division by 2 the result is: %d", res.Value)

	mem, err = client.Memorize(context.Background(), calculator.Tag{Name: "second"})
	if err != nil {
		log.Fatal("Could not memorize with tag second: %v", err)
	}
	log.Printf("memory second: %v", mem)

	dump, err := client.MemoryDump(context.Background(), calculator.Empty{})
	if err != nil {
		log.Fatalf("Could not dump memory: %v", err)
	}
	log.Printf("memory dump: %v", dump)

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

	// Exit
	os.Exit(0)
}
