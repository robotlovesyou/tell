#ifndef TELL_GEN_GOTEMPLATE_H_
#define TELL_GEN_GOTEMPLATE_H_

static const char *go_template = R"TEMPLATE(package {{go_package(directive_declarations)}}

import (
  "time"
)

var _ = time.Now() // Prevent import errors

{%for message in message_declarations%}{%if existsIn(message, "doc_comment") and has_content(message.doc_comment)%}// {{message.doc_comment}}
{%endif%}type {{camel_case(message.name)}} struct {
  {%if existsIn(message, "fields")%}{%for field in message.fields%}{%if existsIn(field, "doc_comment") and has_content(field.doc_comment)%}
  // {{field.doc_comment}}
  {%endif%}
  {{camel_case(field.name)}} {{map_type(field.type_def)}} `json:"{{field.name}},omitempty"`
  {%endfor%}{%endif%}
}

{%endfor%}
)TEMPLATE";

#endif //TELL_GEN_GOTEMPLATE_H_
