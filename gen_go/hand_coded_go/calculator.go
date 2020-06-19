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

type Result {
    value int64
}

type Reset {
}

type TellServer struct {
    mux *http.Mux
}