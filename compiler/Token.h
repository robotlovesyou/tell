#ifndef TELL_COMPILER_TOKEN_H_
#define TELL_COMPILER_TOKEN_H_

#include <string>

namespace til {

static const char *kKeywordMessage = "message";
static const char *kKeywordList = "list";
static const char *kKeywordMap = "Map";
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
    kMessage = 11, kList = 12, kMap = 13, kService = 14, kCall = 15,

    // String Termination
    kLineFeed = 16,

    // File Termination
    kEOF = 17,
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
  std::string TypeName();

  /**
   * Return a valid string literal for the token
   * @return a valid literal for the token
   */
  std::string Literal() const;
};

} // namespace til
#endif //TELL_COMPILER_TOKEN_H_
