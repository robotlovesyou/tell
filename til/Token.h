#ifndef TELL_COMPILER_TOKEN_H_
#define TELL_COMPILER_TOKEN_H_

#include <string>

namespace til {

static const char *kKeywordMessage = "message";
static const char *kKeywordList = "list";
static const char *kKeywordMap = "map";
static const char *kKeywordService = "service";
static const char *kKeywordFloat = "float";
static const char *kKeywordInt = "int";
static const char *kKeywordBool = "bool";
static const char *kKeywordString = "string";
static const char *kKeywordTime = "time";

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
    kString, kDocString,

    // Identifiers
    kIdent,

    // Punctuation marks
    kBang, kColon, kQMark, kLBrace, kRBrace, kLSqBracket, kRSqBracket, kFloat, kInt, kBool, kStringWord, kTime,

    // Keywords
    kMessage, kList, kMap, kService,

    // String Termination
    kLineFeed,

    // File Termination
    kEOF,
  };

  Token() = default;
  Token(Type tp, int l, int c, std::string r);

  // Delete the copy constructor and copy assignment
  // because tokens can contain long strings so
  // copying them is inefficient
  Token(const Token &) = delete;
  Token &operator=(const Token &) = delete;

  // Explicitly define the move and move assignment
  Token(Token && source) noexcept;
  Token &operator=(Token &&source) noexcept;

  ~Token() = default; // The default delete constructor is fine because there is no resource ownership.

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
  std::string TypeName() const;

  /**
   * Return a valid string literal for the token
   * @return a valid literal for the token
   */
  std::string Literal() const;
};

} // namespace til
#endif //TELL_COMPILER_TOKEN_H_
