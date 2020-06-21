#include "ParsingException.h"

#include <utility>

const char *til::ParsingException::what() const noexcept{
  return message_.c_str();
}

til::ParsingException::ParsingException(std::string message): message_(std::move(message)) {

}
