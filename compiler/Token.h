#ifndef TELL_COMPILER_TOKEN_H_
#define TELL_COMPILER_TOKEN_H_

#include <string>

namespace til {

static const char *kKeywordMessage = "message";
static const char *kKeywordList = "list";
static const char *kKeywordService = "service";
static const char *kKeywordCall = "call";

/**
 * Represents a single token from the lexer
 */
struct Token {
 public:

  /**
   * Describes the possible types of tokens
   */
  enum Type {
    // String types
        kString = 1, kDocString = 2,

    // Identifiers
        kIdent = 3,

    // Punctuation marks
        kBang = 4, kColon = 5, kQMark = 6, kLBrace = 7, kRBrace = 8, kLBracket = 9, kRBracket = 10,

    // Keywords
        kMessage = 11, kList = 12, kService = 13, kCall = 14,
    // String Termination
        kLineFeed = 15,

    // File Termination
        kEOF = 16,
  };
  /**
   * The Type of the token
   */
  Type t;

  /**
   * The source line of the start of the token
   */
  int line;

  /**
   * The source column of the start of the token
   */
  int col;

  /**
   * The string representation of the value of the token
   */
  std::string repr;

  /**
   * Return a string containing the type name of the token
   * @return a type name for the toke
   */
  std::string TypeName();

  /**
   * Return a valid string literal for the token
   * @return a valid literal for the token
   */
  std::string Literal() const;
};

} // namespace til
#endif //TELL_COMPILER_TOKEN_H_
