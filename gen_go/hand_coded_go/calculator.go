package calculator

import (
    "time"
)

_ := time.Now() // prevent the import of time causing an issue if there are no time members

type Add struct {
    Addend int64
}

type Subtract struct {
    Subtrahend int64
}

type Multiply {
    Multiplicand int64
}

type Divide {
    Divisor int64
}

type Result {
    Value int64
}

type Empty struct {
}

type Key struct {
    Name string
}

type Memory struct {
    Name string
    Value string
    at: time.Time
}

type Memories struct {
    Entries []Memory
}

type Input struct {
    Key string
    Value int64
}

type Program {
    Operations []map[string]string
}