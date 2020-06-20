#ifndef TELL_GEN_GOTEMPLATE_H_
#define TELL_GEN_GOTEMPLATE_H_

/**
 * This is the inja template for generating a Go representation of the described RPC
 */
static const char *go_template = R"TEMPLATE(package {{go_package(directive_declarations)}}

import (
    "context"
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"time"
)

var _ = time.Now() // Prevent import errors

/********************************************************************************
CORE TELL CODE.

In a production implementation, this should be in a shared library
imported by the generated code.
********************************************************************************/
type ErrorCode = string
const (
	CodeMalformedRequest ErrorCode = "malformed_request"
	CodeInternal = "internal"
)

type TellError struct {
	Code ErrorCode
	Messages map[string]string
}

func (err TellError) Error() string {
	return err.Code
}

func (e TellError) Is(err error) bool {
	tellErr, ok := err.(TellError)
	if !ok {
		return false
	}
	return tellErr.Code == e.Code
}

func MalformedRequestError() error {
	return TellError{Code: CodeMalformedRequest}
}

func InternalError() error {
	return TellError{Code: CodeInternal}
}

type TellServer struct {
	Mux *http.ServeMux
}

func NewTellServer() *TellServer {
	return &TellServer{Mux: http.NewServeMux()}
}

func (server *TellServer) Start(port int) {
	httpServer := http.Server{
		Addr: fmt.Sprintf(":%d", port),
		Handler: http.HandlerFunc(server.Mux.ServeHTTP),
	}
	err := httpServer.ListenAndServe()
	log.Printf("server stopped: %v", err)
}

func writeError(err error, w http.ResponseWriter) {
	log.Printf("error serving request: %v", err)
	var tellErr TellError
	if !errors.As(err, &tellErr) {
		tellErr = InternalError().(TellError)
	}
	w.WriteHeader(http.StatusBadRequest)
	encoder := json.NewEncoder(w)
	_ = encoder.Encode(tellErr)
}

/********************************************************************************
CORE TELL CODE ENDS
********************************************************************************/

/********************************************************************************
GENERATED STRUCTS
********************************************************************************/
{%for message in message_declarations%}{%if existsIn(message, "doc_comment") and has_content(message.doc_comment)%}// {{message.doc_comment}}
{%endif%}type {{camel_case(message.name)}} struct {
    {%if existsIn(message, "fields")%}{%for field in message.fields%}{%if existsIn(field, "doc_comment") and has_content(field.doc_comment)%}
    // {{field.doc_comment}}
    {%endif%}
    {{camel_case(field.name)}} {{map_type(field.type_def)}} `json:"{{field.name}},omitempty"`
    {%endfor%}{%endif%}
}

{%endfor%}
/********************************************************************************
GENERATED STRUCTS END
********************************************************************************/
)TEMPLATE";

#endif //TELL_GEN_GOTEMPLATE_H_
