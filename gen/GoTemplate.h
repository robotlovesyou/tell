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
    CodeInternal = "internal"
    CodeInvalidArgument = "invalid_argument"
	CodeMalformedRequest ErrorCode = "malformed_request"
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

func InvalidArgumentError(messages map[string]string) error {
    return TellError{Code: CodeInvalidArgument, Messages: messages}
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
    w.Header().Add("Content-Type", "application/json")
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
{%for message in message_declarations%}{%if has_content(message.doc_comment)%}// {{message.doc_comment}}
{%endif%}
type {{camel_case(message.name)}} struct {
    {%for field in message.fields%}
    {%if existsIn(field, "doc_comment") and has_content(field.doc_comment)%}
    // {{field.doc_comment}}
    {%endif%}
    {{camel_case(field.name)}} {{map_type(field.type_def)}} `json:"{{field.name}}"`
    {%endfor%}
}

{%endfor%}
/********************************************************************************
GENERATED STRUCTS END
********************************************************************************/

/********************************************************************************
GENERATED SERVER STUBS
********************************************************************************/
{%for service in service_declarations%}
{%if has_content(service.doc_comment)%}// {{service.doc_comment}}
{%endif%}
type {{camel_case(service.name)}}Server interface {
    {%for call in service.calls%}
    {%if has_content(call.doc_comment)%}
    // {{call.doc_comment}}
    {%endif%}
    {{camel_case(call.name)}}(ctx context.Context, arg {{camel_case(call.argument)}}) ({{camel_case(call.returns)}}, error)
    {%endfor%}
}

type {{lower_camel_case(service.name)}}Server_TellService struct {
    server {{camel_case(service.name)}}Server
}

func (service *{{lower_camel_case(service.name)}}Server_TellService) register(tellServer *TellServer) {
    {%for call in service.calls%}
    tellServer.Mux.HandleFunc("/{{snake_case(service.name)}}.{{snake_case(call.name)}}", func(w http.ResponseWriter, r *http.Request) {
		if r.Method != http.MethodPost {
			log.Printf("request is not a post")
			writeError(MalformedRequestError(), w)
			return
		}

		body, err := ioutil.ReadAll(r.Body)
		if err != nil {
			log.Printf("error reading request body: %v", err)
			writeError(InternalError(), w)
			return
		}

		var arg {{camel_case(call.argument)}}
		err = json.Unmarshal(body, &arg)
		if err != nil {
			log.Printf("error unmarshaling body: %v", err)
			writeError(MalformedRequestError(), w)
			return
		}

		resp, err := service.server.{{camel_case(call.name)}}(r.Context(), arg)
		if err != nil {
			log.Printf("error calling server.{{camel_case(call.name)}}: %v", err)
			writeError(err, w)
			return
		}
		encoder := json.NewEncoder(w)
        w.Header().Add("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		_ = encoder.Encode(&resp)
	})

    {%endfor%}
}

func Register{{camel_case(service.name)}}Server({{lower_camel_case(service.name)}}Server {{camel_case(service.name)}}Server, tellServer *TellServer) {
    service := &{{lower_camel_case(service.name)}}Server_TellService {
        server: {{lower_camel_case(service.name)}}Server,
    }
    service.register(tellServer)
}
{%endfor%}

/********************************************************************************
GENERATED SERVER STUBS END
********************************************************************************/
)TEMPLATE";

#endif //TELL_GEN_GOTEMPLATE_H_
