#ifndef TELL_GEN_GOTEMPLATE_H_
#define TELL_GEN_GOTEMPLATE_H_

static const char *go_template = R"TEMPLATE(package {{go_package(data.directive_declarations}}

import (
  "time"
)

_ = Time.now() // Prevent import errors
)TEMPLATE";

#endif //TELL_GEN_GOTEMPLATE_H_
